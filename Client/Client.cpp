#include "Client.h"

using namespace std;
Client::Client()
{
	IO_handler = new boost::asio::io_service();
	socket_forClient = new boost::asio::ip::tcp::socket(*IO_handler);
	client_resolver = new boost::asio::ip::tcp::resolver(*IO_handler);

	error_.type == N_ERROR;
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
		error_.type == CONNECTION_ERROR;
		error_.errStr = string("Error while trying to listen to ") + HELLO_PORT_STR + "Port " + error.message();

		/*cout << "Error connecting to " << host << " Error Message: " << error.message() << endl;
		if (error.value() == boost::asio::error::connection_refused)
			cout << "Host " << host << " is not listening on the other side" << endl;*/
	}
	socket_forClient->non_blocking(true);
}

void Client::generateStringToSend(parseString route)
{
	messageToServer = "GET " + string(route.hostName) + string(route.pathRoute) + "HTTP/1.1 \r\nHost : 127.0.0.1 \r\n\r\n";
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
		boost::system::error_code error;
		size_t len = 0;
		cout << "Receiving Message" << std::endl;
		boost::timer::cpu_timer t;
		t.start();
		boost::timer::cpu_times pastTime = t.elapsed();
		double elapsedSeconds = 0.0;

		do
		{
			len = socket_forClient->read_some(boost::asio::buffer(messageFromServer), error);

			boost::timer::cpu_times currentTime = t.elapsed();

			if ((currentTime.wall - pastTime.wall) > 1e9)
			{
				elapsedSeconds += (currentTime.wall - pastTime.wall) / 1e9;
				pastTime = currentTime;
				cout << "Pasaron " << (int)floor(elapsedSeconds) << " segundos." << endl;
			}

			//if (!error)


		} while (error.value() == WSAEWOULDBLOCK);

		if (!error)
		{
			fputs(messageFromServer.c_str(), file);
			error_.errStr = string("Succes reading from server");
		}
		else
		{
			error_.type = CONNECTION_ERROR;
			error_.errStr = string("Error while trying to recive massage. ") + error.message();
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


int
main(int argc, char* argv[])
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
		free(route.hostName);
		free(route.pathRoute);
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
	string::size_type pos = s.find_first_of('/', 0);
	if (pos != string::npos)	//busco el primer '/'
	{
		if ((route.hostName = (char *)malloc(pos * sizeof(char))) != NULL)
		{
			s.copy(route.hostName, pos, 0);		//no guardo valor de retorno por el metodo copy ya que no me interesa saber hasta que posicion llego pues es la misma que pos
			route.hostName[pos] = '\0';			//cargo null terminated... notar que sobre escribo al '/'
			string::size_type finalSlashPos = s.find_last_of('/', string::npos);		//busco el ultimo separador
			if ((finalSlashPos != string::npos) && (finalSlashPos != (s.length() - 1)))		//me fijo que el path no termine con una barra 
			{
				if ((route.pathRoute = (char *)malloc((s.length() - pos) * sizeof(char))) != NULL)
				{
					s.copy(route.pathRoute, s.length(), pos);
					route.pathRoute[s.length() - pos] = '\0';
					route.validation = true;
					return route;
				}
			}
			free(route.hostName);
		}
	}
	route.validation = false;
	return route;
}