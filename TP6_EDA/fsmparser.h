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
	HTTP_STRING_EVENT,
	DOUBLECRLF_EVENT,
	OTHER_EVENT

}FSM_EVENTS;

#define FSM_EV_COUNT 4

typedef enum
{
	WAIT_FOR_GET_STATE,
	WAIT_FOR_HOST_STATE,
	WAIT_FOR_END_STATE,
	END_STATE
}FSM_STATES;

#define FSM_ST_COUNT 4

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

	fsmparser(char* str2parse_);
	~fsmparser();
	bool parse(void);

	string getHost(void);
	string getPath(void);
	char* getString(void);
	bool getError();

	void setError(void);
	void setHost(char* p2token_);
	void setPath(char* p2token_);



private:

	string host;
	string path;
	char* str2parse;
	FSM_STATES currState;
	bool error;

	event_t eventGen(char* pointer); // devuelve el evento

	void cycleFSM(event_t ev);

	const cells_t fsmArray[FSM_ST_COUNT][FSM_EV_COUNT]
	{ // GET	HTTPSTRING		DOUBLECRLF  OTHER_EVENT
		{{savePath,WAIT_FOR_HOST_STATE},{fsmError,END_STATE},{fsmError,END_STATE},{fsmError, END_STATE}}, //WAIT_FOR_GET_STATE
		{{fsmError,END_STATE},{saveHost,WAIT_FOR_END_STATE},{fsmError,END_STATE},{fsmError,END_STATE}}, //WAIT_FOR_HOST_STATE
		{{fsmError,END_STATE},{fsmError,END_STATE},{NULL,END_STATE},{fsmError,END_STATE}}, //WAIT_FOR_END_STATE
		{{NULL,END_STATE},{NULL,END_STATE},{NULL,END_STATE},{NULL,END_STATE}},//END_STATE
	};


};

