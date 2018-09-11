#include "fsmparser.h"
using namespace std;

/*
fsmparser()
constructor del objeto fsmparser
*/

fsmparser::
fsmparser(string* str2parse_)
{
	error = false; // no hay error
	host = "NULL";
	path = "NULL";
	currState = WAIT_FOR_GET_STATE; // estado inicial
	str2parse = str2parse_;
	lastPointer = 0;

}


/*
fsmparser()
destructor del objeto fsmparser
*/



/*
getError()
getter del error
*/

bool fsmparser::
getError(void)
{
	return error;
}

unsigned int 
fsmparser::getOffset(void)
{
	return lastPointer;
}


/*
getHost()
getter del host
*/

string fsmparser::
getHost(void)
{
	return host;
}

/*
getPath()
getter del path
*/

string fsmparser::
getPath(void)
{
	return path;
}

string * fsmparser::
getString(void)
{
	return str2parse;
}

/*
setError()
setter del error
*/
void fsmparser::
setError(void)
{
	error = true;
}
/*
setHost()
setter de la variable string host
*/
void fsmparser::
setHost(string* p2token_)
{
	host = *p2token_;
}

void fsmparser::
setPath(string * p2token_)
{
	path = *p2token_;
}


/*
* eventGen()
* método generador de eventos de la fsm
*/

event_t fsmparser::
eventGen(void)
{
	event_t ev;
	int newPointer = 0;
	string get = "GET ";
	string http = "HTTP/1.1" CRLF "Host:";
		if ((newPointer = str2parse->find(get,lastPointer)) != string::npos)
		{
			ev.currentEv = GET_EVENT;
			lastPointer = (newPointer + 5); // set del nuevo offset, sumando el largo del string buscado
		}

		else if ((newPointer = str2parse->find("HTTP/1.1\u000d\u000aHost:", lastPointer)) != string::npos)
		{
			ev.currentEv = HTTP_STRING_EVENT;
			lastPointer = (newPointer + 15); // set del nuevo offset, sumado el largo del string buscado
		}


		else
		{
			ev.currentEv = OTHER_EVENT;
		}

		return ev;
}

/*
cycleFSM()
método que cicla la FSM, llamando a la rutina de acción correspondiente y estableciendo el siguiente estado
*/
void fsmparser::
cycleFSM(event_t ev)
{
	if (fsmArray[currState][ev.currentEv].actRout != NULL) // si hay rutina de acción definida para el par evento/estado...
	{
		fsmArray[currState][ev.currentEv].actRout(this); // llamo a la rutina de acción con un puntero al objeto
	}

	currState = (FSM_STATES)fsmArray[currState][ev.currentEv].nextState; // escribo siguiente estado
}

/*
 parse()
 método que corre la FSM
*/
bool fsmparser::
parse(void)
{
	while ((currState != END_STATE) && (error == false)) // mientras no llegué al final, y no hubo error
	{
		cycleFSM(eventGen()); // ciclo la fsm
	}

	return error;
}

/********** RUTINAS DE ACCION DE LA FSM *********/

void fsmError(void* pointer)
{
	fsmparser* p2obj = (fsmparser*)pointer;
	p2obj->setError();
}

void savePath(void* pointer)
{
	fsmparser* p2obj = (fsmparser*)pointer;
	unsigned int lastPointer = p2obj->getOffset();
	unsigned int spacePos = p2obj->getString()->find(" ", lastPointer);
	string str = p2obj->getString()->substr(lastPointer, spacePos - lastPointer); // obtengo el substring con lo necesario

	if (spacePos != string::npos)
	{
		p2obj->setPath(&str); // guardo el path en la variable indicada (sumo 1 para omitir la barra)
	}

	else
	{
		p2obj->setError(); // hubo error en el parseo
	}
}

void saveHost(void* pointer)
{
	fsmparser* p2obj = (fsmparser*)pointer;
	unsigned int lastPointer = p2obj->getOffset();
	unsigned int spacePos = p2obj->getString()->find(CRLF, lastPointer);
	string str = p2obj->getString()->substr(lastPointer, spacePos - lastPointer); // obtengo el substring con lo necesario

	if (spacePos != string::npos)
	{
		p2obj->setHost(&str); // guardo el path en la variable indicada (sumo 1 para omitir la barra)

	}

	else
	{
		p2obj->setError(); // hubo error en el parseo
	}

}
