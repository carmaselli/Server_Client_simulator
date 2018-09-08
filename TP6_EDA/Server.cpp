#include "Server.h"
#include<ctime>




#define LF 0x0A  //line feed
#define CR 0x0D  //carriage return

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

void Server::sendMessage() //esta hay que cambiar
{
	if (error_.type == NO_ERROR)
	{
		char buf[512] = "Hello from server.";

		size_t len;
		boost::system::error_code error;

		do
		{
			len = socket_forServer->write_some(boost::asio::buffer(buf, strlen(buf)), error);
		} while ((error.value() == WSAEWOULDBLOCK));
		if (error)
			std::cout << "Error while trying to connect to client " << error.message() << std::endl;
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

void Server::receiveMessage()
{
	boost::system::error_code error;
	char buf[512];
	size_t len = 0;
	cout << "Receiving Message" << std::endl;
	boost::timer::cpu_timer t;
	t.start();
	boost::timer::cpu_times pastTime = t.elapsed();
	double elapsedSeconds = 0.0;

	do
	{
		len = socket_forServer->read_some(boost::asio::buffer(buf), error);

		boost::timer::cpu_times currentTime = t.elapsed();

		if ((currentTime.wall - pastTime.wall) > 1e9)
		{
			elapsedSeconds += (currentTime.wall - pastTime.wall) / 1e9;
			pastTime = currentTime;
			cout << "Pasaron " << (int)floor(elapsedSeconds) << " segundos." << endl;
		}

		if (!error)
			buf[len] = '\0';

	} while (error.value() == WSAEWOULDBLOCK);

	if (!error)
		cout << std::endl << "Server sais: " << buf << std::endl;
	else
		cout << "Error while trying to connect to server " << error.message() << std::endl;

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
