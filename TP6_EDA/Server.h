#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>
#include"error.h"
#include"fsmparser.h"

#define HELLO_PORT 80
using namespace std;

class Server
{
public:
	Server();
	void startConnection();
	void sendMessage();
	void sendMessage(const char* message);
	void receiveMessage();


	bool getParseOk();
	error_t getError();
	~Server();
private:
	error_t error_;
	string htmlFileContent;
	string messageForClient;
	string timestamp;
	string timestampExp;
	bool parseOk;


	boost::asio::io_service*  IO_handler;
	boost::asio::ip::tcp::socket* socket_forServer;
	boost::asio::ip::tcp::acceptor* server_acceptor;
	void fillMessage(fsmparser& Parser);
	bool isFilePresent(const char * path);
	long int fileLength(FILE* filename);
	void readFile(FILE* filename);
	void fillTimestamps();
	
};
