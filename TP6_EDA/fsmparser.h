#pragma once
#include <string>
#include <iostream>
#include <cstdio>
#include <cstring>


using namespace std;


/* CONSTANTES*/

#define SPACE " "
#define CRLF "1310"
#define TWOPOINTS ":"
/* TYPEDEFS */
typedef struct
{
	char* token;
	FSM_EVENTS  currentEv;
}event_t;

typedef enum 
{
	GET_EVENT,
	SPACE_EVENT,
	HTTP_STRING_EVENT,
	DOUBLECRLF_EVENT,
	OTHER_EVENT

}FSM_EVENTS;

#define FSM_EV_COUNT 5

typedef enum
{
	WAIT_FOR_GET_STATE,
	SAVE_PATH_STATE,
	WAIT_FOR_HOST_STATE,
	SAVE_HOST_STATE,
	WAIT_FOR_END_STATE,
	END_STATE
}FSM_STATES;

#define FSM_ST_COUNT 5

typedef void(*actrout_t)(void*);

typedef struct
{
	actrout_t actRout;
	FSM_STATES nextState;
}cells_t;


/* CLASS */

class fsmparser
{
public:

	fsmparser();
	~fsmparser();
	bool parse(char* str2parse);
	string getHost(void);
	string getPath(void);


private:

	string host;
	string path;
	FSM_STATES currState;
	bool error;

	event_t eventGen(char* pointer); // devuelve el evento
	bool getError();
	void cycleFSM(event_t ev);

	const cells_t fsmArray[FSM_ST_COUNT][FSM_EV_COUNT]
	{ // GET	SPACE	HTTPSTRING		DOUBLECRLF  OTHER_EVENT
		{NULL,SAVE_PATH_STATE},{fsmError,END_STATE},{fsmError,END_STATE},{fsmError,END_STATE},{}, //WAIT_FOR_GET_STATE
		{fsmError,END_STATE},{},{},{},{}, //SAVE_PATH_STATE
		{},{},{},{},{}, //WAIT_FOR_HOST_STATE
		{},{},{},{},{}, //SAVE_HOST_STATE
		{},{},{},{},{}, //WAIT_FOR_END_STATE
		{},{},{},{},{},//END_STATE
	};


};

