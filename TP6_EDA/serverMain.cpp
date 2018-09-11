#include<iostream>
#include<string>
#include"Server.h"
#include"error.h"
#include<conio.h>

void wrapper(bool& keepGoing);

int main(int argc, char* argv[])
{
	
	bool keepGoing = true;
	while (keepGoing)
	{
		wrapper(keepGoing);
	}
	cout << "Press enter to exit..." << endl;
	while (getch() != '\n');
	return EXIT_SUCCESS;
}


void wrapper(bool& keepGoing)
{
	Server myServer;
	std::cout << std::endl << "Start Listening on port " << HELLO_PORT << std::endl;
	myServer.startConnection();
	std::cout << "Somebody connected to port " << HELLO_PORT << std::endl;

	do
	{
		myServer.receiveMessage();
	} while (!myServer.getParseOk());
	myServer.sendMessage();
	Sleep(50); // Le damos 50ms para que llegue el mensaje antes de volver a esperar 
	if (myServer.getError().type != N_ERROR)
	{
		cout << myServer.getError().errStr;
		keepGoing = false;
	}
	//myServer.~Server(); // destruyo el server

	
}