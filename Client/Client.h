#pragma once
#include <iostream>
#include <cstdio>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>
#include <cstring>

#include <cstdlib>
#include "error.h"

#define HELLO_PORT_STR "80"
#define SERVER_IP "localhost"

#define PATH "C:\\Users\\fliatondi\\source\\repos\\Server_Client_simulator\\TP6_EDA"

#define MESSAGE(x) "GET " (x) "HTTP/1.1 \r\nHost : 127.0.0.1 \r\n\r\n"


typedef struct
{
	string hostName;
	string pathRoute;
	bool validation;
}parseString;



class Client
{
public:
	Client();
	void startConnection(const char* host);
	void sendMessage(void);
	void generateStringToSend(parseString route);
	void receiveMessage();
	~Client();

	error_t getError();

private:
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* socket_forClient;
	boost::asio::ip::tcp::resolver* client_resolver;
	boost::asio::ip::tcp::resolver::iterator endpoint;
	
	error_t error_;
	string messageToServer;
	string messageFromServer;
	FILE *file;
};