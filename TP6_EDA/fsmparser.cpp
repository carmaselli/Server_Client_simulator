#include "fsmparser.h"
using namespace std;

/*
fsmparser()
constructor del objeto fsmparser
*/

fsmparser::
fsmparser(const char* str2parse_)
{
	error = false; // no hay error
	host = "NULL";
	path = "NULL";
	currState = WAIT_FOR_GET_STATE; // estado inicial
	str2parse = (char*)malloc(strlen(str2parse_));
	if (str2parse == NULL)
	{
		error = true;
	}
}


/*
fsmparser()
destructor del objeto fsmparser
*/

fsmparser::
~fsmparser(void)
{
	free(str2parse);
}


/*
getError()
getter del error
*/

bool fsmparser::
getError(void)
{
	return error;
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

char * fsmparser::
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
setHost(char* p2token_)
{
	string s(p2token_);
	host = s;
}

void fsmparser::
setPath(char * p2token_)
{
	string s(p2token_);
	path = s;
}

/*
* eventGen()
* método generador de eventos de la fsm
*/

event_t fsmparser::
eventGen(char* pointer)
{
	event_t ev;
	ev.token = strtok(pointer, SPACE); // genero token, en función del estado actual

	if (ev.token != NULL)
	{
		if (strcmp((const char *)ev.token, "GET"))
		{
			ev.currentEv = GET_EVENT;
		}

		else if (strcmp((const char *)ev.token, " HTTP/1.1 " CRLF "Host:"))
		{
			ev.currentEv = HTTP_STRING_EVENT;
		}


		else
		{
			ev.currentEv = OTHER_EVENT;
		}

	}
	else
	{
		error = true;
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
	while ( (currState != END_STATE) && (error == false)) // mientras no llegué al final, y no hubo error
	{
		cycleFSM(eventGen(str2parse)); // ciclo la fsm
	}

	return error;
}

/********** RUTINAS DE ACCION DE LA FSM *********/

void fsmError(void* pointer)
{
	fsmparser* p2obj = (fsmparser*) pointer;
	p2obj->setError();
}

void savePath(void* pointer)
{
	fsmparser* p2obj = (fsmparser*)pointer;
	char* p2token = strtok(p2obj->getString(), SPACE);

	if (p2token != NULL)
	{
		p2obj->setPath(p2token); // guardo el path en la variable indicada
		
	}

	else
	{
		p2obj->setError(); // hubo error en el parseo
	}
}

void saveHost(void* pointer)
{
	fsmparser* p2obj = (fsmparser*)pointer;
	char* p2token = strtok(p2obj->getString(), SPACE);

	if (p2token != NULL)
	{
		p2obj->setHost(p2token); // guardo el host en la variable indicada
	}

	else
	{
		p2obj->setError(); // hubo error en el parseo
	}

}