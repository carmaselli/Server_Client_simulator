#pragma once
#include <iostream>
#include <cstdio>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>

#define HELLO_PORT_STR "80"
#define SERVER_IP "localhost"

#define PATH "\/path"
#define FILENAME "\/filename"

#define MESSAGE "GET " PATH FILENAME "HTTP/1.1 \r\nHost : 127.0.0.1 \r\n\r\n"
	
parseString clientParser(int argc, char* argv[]);

typedef struct
{
	char *hostName;
	char *pathRoute;
	bool validation;
}parseString;

class Client
{
public:
	Client();
	void startConnection(const char* host);
	void sendMessage();
	void receiveMessage();
	~Client();
private:
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* socket_forClient;
	boost::asio::ip::tcp::resolver* client_resolver;
	boost::asio::ip::tcp::resolver::iterator endpoint;

};