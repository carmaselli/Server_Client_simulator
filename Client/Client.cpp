#include "Client.h"

using namespace std;
Client::Client()
{
	IO_handler = new boost::asio::io_service();
	socket_forClient = new boost::asio::ip::tcp::socket(*IO_handler);
	client_resolver = new boost::asio::ip::tcp::resolver(*IO_handler);
}

void Client::startConnection(const char * host)
{
	endpoint = client_resolver->resolve(boost::asio::ip::tcp::resolver::query(host, HELLO_PORT_STR));
	boost::system::error_code error;
	boost::asio::connect(*socket_forClient, endpoint, error);
	if (error)
	{
		cout << "Error connecting to " << host << " Error Message: " << error.message() << endl;
		if (error.value() == boost::asio::error::connection_refused)
			cout << "Host " << host << " is not listening on the other side" << endl;
	}
	socket_forClient->non_blocking(true);
}

void Client::receiveMessage()
{
	boost::system::error_code error;
	char buf[512];
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
			buf[len] = '\0';

	} while (error.value() == WSAEWOULDBLOCK);

	if (!error)
		cout << std::endl << "Server sais: " << buf << std::endl;
	else
		cout << "Error while trying to connect to server " << error.message() << std::endl;

}

Client::~Client()
{
	socket_forClient->close();
	delete client_resolver;
	delete socket_forClient;
	delete IO_handler;
}


int
main(int argc, char* argv[])
{
	Client conquering;
	cout << "Trying to connect to " << SERVER_IP << " on port " << HELLO_PORT_STR << std::endl;
	conquering.startConnection(SERVER_IP);
	conquering.receiveMessage();
	cout << "Press Enter to exit..." << std::endl;
	getchar();
	return 0;
}

//crear funcion de parseo para interpretar lo que me pasan por linea de ccomando y desde el main pasarle 
//al objeto lo leido
//una vez recibido el buffer (buf) copiar todo a un .txt

parseString clientParser(int argc, const char *argv[])	//falta analizar cuando ocurren errores... ver que es npos
{
	parseString route;
	string s(argv[0]);		//copio el contenido del arreglo en un string
	string::size_type pos = s.find_first_of('/', 0);	//busco el primer '/'
	route.hostName = (char *) malloc(pos * sizeof(char));
	s.copy(route.hostName, pos, 0);		//no guardo valor de retorno ya que no me interesa saber hasta que posicion llego pues es la misma que pos
	route.hostName[pos] = '\0';			//cargo null terminated... notar que sobre escribo al '/'
	string::size_type pos2 = s.find_first_of('\0', pos);
	route.pathRoute = (char *)malloc(pos2 * sizeof(char));
	s.copy(route.pathRoute, pos2, pos);

	//faltan validaciones
	return route;
}