#pragma once


#include<string>
using namespace std;

typedef enum { N_ERROR, CONNECTION_ERROR, PARSER_CONSTRUCTOR_ERROR}errorType;/*aca van los tipos de error que pueden ocurrir*/
typedef struct
{
	errorType type;
	string errStr;
}error_t;