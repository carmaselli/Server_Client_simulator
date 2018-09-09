#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>
#include"error.h"

#define HELLO_PORT 80


class Server
{
public:
	Server();
	void startConnection();
	void sendMessage();
	void sendMessage(const char* message);
	void receiveMessage();

	error_t getError();
	~Server();
private:
	error_t error_;
	string htmlFileContent;
	string messageForClient;


	boost::asio::io_service*  IO_handler;
	boost::asio::ip::tcp::socket* socket_forServer;
	boost::asio::ip::tcp::acceptor* server_acceptor;
	bool readFile(const char* path);
	void fillMessage();
	
};
