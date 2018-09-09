#include "Server.h"
#include<ctime>
#include <boost/filesystem.hpp>






Server::Server()
{
	error_.type = N_ERROR;
	IO_handler = new boost::asio::io_service();
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), HELLO_PORT);

	socket_forServer = new boost::asio::ip::tcp::socket(*IO_handler);
	server_acceptor = new boost::asio::ip::tcp::acceptor(*IO_handler, ep);

}

void Server::startConnection()
{

	//Uncomment if non-blocking mode is desired
	//
	//When non-blocking mode is chosen accept operation
	//will fail with boost::asio::error::would_block
	//if there's no client inmediatelly connected when accept operation is performed.
	//Must comment blocking mode.
	//
	server_acceptor->non_blocking(true);
	boost::system::error_code error;
	do
	{
		server_acceptor->accept(*socket_forServer, error);
	} while ((error.value() == WSAEWOULDBLOCK));
	if (error)
	{
		error_.type = CONNECTION_ERROR;
		error_.errStr = string("Error while trying to listen to ") + to_string(HELLO_PORT) + "Port " + error.message();
	}
	//server_acceptor->accept(*socket_forServer);
	socket_forServer->non_blocking(true);
}

void Server::sendMessage() 
{
	if (error_.type == NO_ERROR)
	{
		size_t len;
		boost::system::error_code error;

		do
		{
			len = socket_forServer->write_some(boost::asio::buffer(messageForClient.c_str(), messageForClient.length()), error);
		} while ((error.value() == WSAEWOULDBLOCK));
		if (error)
		{
			error_.type = CONNECTION_ERROR;
			error_.errStr = string("Error while trying to connect to client ") + error.message();
		}
	}
}

void Server::sendMessage(const char * message)
{
	size_t len;
	boost::system::error_code error;

	do
	{
		len = socket_forServer->write_some(boost::asio::buffer(message, strlen(message)), error);
	} while ((error.value() == WSAEWOULDBLOCK));
	if (error)
	{
		error_.type = CONNECTION_ERROR;
		error_.errStr = string("Error while trying to connect to client ") + error.message();
	}
}

void Server::receiveMessage() //falta cambiar esto
{
	boost::system::error_code error;
	char buf[512];
	size_t len = 0;
	string receivedMessage;
	cout << "Receiving Message" << std::endl;
	boost::timer::cpu_timer t;
	t.start();
	boost::timer::cpu_times pastTime = t.elapsed();
	double elapsedSeconds = 0.0;


	

	do   //espero recibir algo
	{
		len = socket_forServer->read_some(boost::asio::buffer(buf), error);

		if (!error)
			buf[len] = '\0';
		receivedMessage += string(buf);

	} while (error.value() == WSAEWOULDBLOCK);

	if (!error)
	{
		fsmparser Parser((receivedMessage.c_str()));
		if (Parser.getError() == false)
		{
			if (!Parser.parse()) //parseo del string
			{
				fillMessage(Parser);
			}
			else
			{
				//ver que hacer aca para que se repita
			}
		}
		else
		{
			error_.type = PARSER_CONSTRUCTOR_ERROR;
			error_.errStr = string("Error while trying to create parser ");
		}
	}
	else
	{
		error_.type = CONNECTION_ERROR;
		error_.errStr = string("Error while trying to connect to server ") + error.message() ;
		
	}
		

}

error_t Server::getError()
{
	return error_;
}

Server::~Server()
{
	server_acceptor->close();
	socket_forServer->close();
	delete server_acceptor;
	delete socket_forServer;
	delete IO_handler;
}

void Server::readFile(FILE* filename)
{
	do
	{
		htmlFileContent += fgetc(filename);
	} while (htmlFileContent.back() != EOF);
	htmlFileContent.pop_back();
}

void Server::fillMessage(fsmparser& Parser)
{
	if (isFilePresent(Parser.getPath().c_str()))
	{
		FILE* htmlFile = fopen(Parser.getPath().c_str(), "rb");
		readFile(htmlFile);
		messageForClient = string("HTTP/1.1 200 OK" CRLF);
		fillTimestamps();
		messageForClient += string("Date: ") + timestamp + CRLF;
		messageForClient += string("Cache-Control: public, max-age=30" CRLF);
		messageForClient += string("Expires: ") + timestampExp + CRLF;
		messageForClient += string("Content-Length: ") + to_string(fileLength(htmlFile)) + CRLF;
		messageForClient += string("Content-Type: text/html; charset=iso-8859-1") + CRLF;
		messageForClient += htmlFileContent + CRLF;
		fclose(htmlFile);
	}
	else
	{
		messageForClient = string("HTTP/1.1 404 Not Found" CRLF);
		fillTimestamps();
		messageForClient += string("Date: ") + timestamp + CRLF;
		messageForClient += string("Cache-Control: public, max-age=30" CRLF);
		messageForClient += string("Expires: ") + timestampExp + CRLF;
		messageForClient += string("Content-Length: 0" CRLF) ;
		messageForClient += string("Content-Type: text/html; charset=iso-8859-1" CRLF)  ;
	}
}

bool Server::isFilePresent(const char * path)
{
	FILE* temp = fopen(path, "rb");
	if (temp != NULL)
	{
		fclose(temp);
		return true;
	}
	else
	{
		return false;
	}
}

long int Server::fileLength(FILE * filename)
{
	fseek(filename, 0, SEEK_END);
	long int fileLength = ftell(filename);
	return fileLength;
}

void Server::fillTimestamps(void)
{
	time_t now = time(0); // get de la fecha actual en time_t
	tm* expDate = gmtime(&now); // cambio el formato de la misma
	time_t nowtime = mktime(expDate);
	timestamp = ctime((const time_t*)&nowtime); // guardo en string la fecha actual
	time_t expiration = mktime(expDate) + (time_t)30; // sumo los 30 segundos
	timestampExp = ctime(&expiration); // guardo en string la fecha de expiración
}