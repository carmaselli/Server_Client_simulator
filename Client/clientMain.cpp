#include <iostream>
#include <cstdio>
#include <cstring>

#include <cstdlib>
#include "error.h"
#include "Client.h"


parseString clientParser(int argc, const char* argv[]);



int
main(int argc, const char* argv[])
{
	Client conquering;

	parseString route;
	route = clientParser(argc, argv);
	if (route.validation == true)
	{

		cout << "Trying to connect to " << SERVER_IP << " on port " << HELLO_PORT_STR << std::endl;
		conquering.startConnection(SERVER_IP);
		if (conquering.getError().type == N_ERROR)
		{
			conquering.generateStringToSend(route);
			conquering.sendMessage();
			conquering.receiveMessage();
		}
		cout << conquering.getError().errStr << endl;
	}
	else
	{
		cout << "Invalid parameters" << endl;
	}
	cout << "Press Enter to exit..." << std::endl;
	getchar();
	return 0;
}



parseString clientParser(int argc, const char *argv[])
{
	parseString route;
	string s(argv[1]);		//copio el contenido del arreglo en un string
	string::size_type pos = s.find_first_of('\\', 0);
	if (pos != string::npos)	//busco el primer '/'
	{
		route.hostName = s.substr(0, pos);
		route.hostName[pos] = '\0';			//cargo null terminated... notar que sobre escribo al '/'
		string::size_type finalSlashPos = s.find_last_of('\\', string::npos);		//busco el ultimo separador
		if ((finalSlashPos != string::npos) && (finalSlashPos != (s.length() - 1)))		//me fijo que el path no termine con una barra 
		{
			route.pathRoute = s.substr(pos, s.length() - pos);
			route.pathRoute[s.length() - pos] = '\0';
			route.validation = true;
			return route;
		}
	}
	route.validation = false;
	return route;
}