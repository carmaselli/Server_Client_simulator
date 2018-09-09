#include "fsmparser.h"
using namespace std;


fsmparser::
fsmparser()
{
	error = false; // no hay error
	host = "NULL";
	path = "NULL";
	currState = WAIT_FOR_END_STATE; // estado inicial

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

/*
* eventGen()
* método generador de eventos de la fsm
*/

event_t fsmparser::
eventGen(char* pointer)
{
	event_t ev = { NULL,NULL };
	ev.token = strtok(pointer, (currState == WAIT_FOR_HOST_STATE)?TWOPOINTS:SPACE); // genero token, en función del estado actual

	if (ev.token != NULL)
	{
		if (strcmp((const char *)ev.token, "GET"))
		{
			ev.currentEv = GET_EVENT;
		}

		else if (strcmp((const char *)ev.token, " HTTP/1.1 1310Host"))
		{
			ev.currentEv = HTTP_STRING_EVENT;
		}

		else if (strcmp((const char *)ev.token, "13101310"))
		{
			ev.currentEv = DOUBLECRLF_EVENT;
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

