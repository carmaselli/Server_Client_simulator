#include<iostream>
#include<string>
#include"Server.h"
#include"error.h"
#include<conio.h>


void hola()
{
	Server myServer;
	std::cout << std::endl << "Start Listening on port " << HELLO_PORT << std::endl;
	myServer.startConnection();
	std::cout << "Somebody connected to port " << HELLO_PORT << std::endl;
	//myServer.sendMessage("Hello from Server");
	//do
	//{
		do
		{
			myServer.receiveMessage();
		} while (!myServer.getParseOk());
		myServer.sendMessage();
		Sleep(50); // Le damos 50ms para que llegue el mensaje antes de volver a esperar (hace falta?)		
	//} while (myServer.getError().type == NO_ERROR);
	cout << myServer.getError().errStr;

}

int main(int argc, char* argv[])
{
	
	while (1)
	{
		hola();
	}
	return EXIT_SUCCESS;
}