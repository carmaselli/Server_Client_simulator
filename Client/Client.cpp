#include "Client.h"
 
using namespace std;
Client::Client()
{
	IO_handler = new boost::asio::io_service();
	socket_forClient = new boost::asio::ip::tcp::socket(*IO_handler);
	client_resolver = new boost::asio::ip::tcp::resolver(*IO_handler);

	error_.type = N_ERROR;
	file = fopen("webService.txt", "wb");
	if (file == NULL)
	{
		error_.type = FILE_ERROR;
		error_.errStr = string("Error opening the file");
	}
}

void Client::startConnection(const char * host)
{
	endpoint = client_resolver->resolve(boost::asio::ip::tcp::resolver::query(host, HELLO_PORT_STR));
	boost::system::error_code error;
	boost::asio::connect(*socket_forClient, endpoint, error);
	if (error)
	{
		error_.type = CONNECTION_ERROR;
		error_.errStr = string("Error while trying to listen to ") + HELLO_PORT_STR + " Port " + error.message();

		/*cout << "Error connecting to " << host << " Error Message: " << error.message() << endl;
		if (error.value() == boost::asio::error::connection_refused)
			cout << "Host " << host << " is not listening on the other side" << endl;*/
	}
	socket_forClient->non_blocking(true);
}

void Client::generateStringToSend(parseString route)
{
	messageToServer = "GET " + route.pathRoute + " HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";
}

void Client::sendMessage(void)
{
	if (error_.type == N_ERROR)
	{
		size_t len;
		boost::system::error_code error;

		do
		{
			len = socket_forClient->write_some(boost::asio::buffer(messageToServer.c_str(), messageToServer.length()), error);
		} while ((error.value() == WSAEWOULDBLOCK));
		if (error)
		{
			//std::cout << "Error while trying to send message from client " << error.message() << std::endl;
			error_.type = CONNECTION_ERROR;
			error_.errStr = string("Error while trying to send massage. ") + error.message();
		}
			
	}
}

void Client::receiveMessage()
{
	if (error_.type == N_ERROR)
	{
		char buf[512];
		boost::system::error_code error;
		size_t len = 0;
		cout << "Receiving Message" << std::endl;
		boost::timer::cpu_timer t;
		t.start();
		boost::timer::cpu_times pastTime = t.elapsed();
		double elapsedSeconds = 0.0;

		do
		{
			len = socket_forClient->read_some(boost::asio::buffer(buf), error);

			boost::timer::cpu_times currentTime = t.elapsed();

			if ((currentTime.wall - pastTime.wall) > 1e9)
			{
				elapsedSeconds += (currentTime.wall - pastTime.wall) / 1e9;
				pastTime = currentTime;
				cout << "Pasaron " << (int)floor(elapsedSeconds) << " segundos." << endl;
			}

			if (!error)
			{
				messageFromServer += buf;
			}


		} while (error.value() == WSAEWOULDBLOCK);

		if (!error)
		{
			messageFromServer[len] = '\0';
			fputs(messageFromServer.c_str(), file);
			printf("%s", messageFromServer.c_str());
			error_.errStr = string("Succes reading from server");
			fflush(file);
		}
		else
		{
			error_.type = CONNECTION_ERROR;
			error_.errStr = string("Error while trying to recive message. ") + error.message();
		}
	}
}

error_t Client::getError()
{
	return error_;
}


Client::~Client()
{
	fclose(file);
	socket_forClient->close();
	delete client_resolver;
	delete socket_forClient;
	delete IO_handler;
}


