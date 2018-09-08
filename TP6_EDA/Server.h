#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
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
	string receivedMessage;

	boost::asio::io_service*  IO_handler;
	boost::asio::ip::tcp::socket* socket_forServer;
	boost::asio::ip::tcp::acceptor* server_acceptor;
	bool parse(char* buffer); //esto no se si vas a hacer una clase joaco, si es asi mandame un msj
	bool readFile(char* path);


};
