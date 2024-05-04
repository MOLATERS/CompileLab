#ifndef INTER_H
#define INTER_H

#include "parser.h"
#include "sematic.h"
#include "methods.h"


typedef struct operand {
	enum{
		OP_VARIABLE,
		OP_CONSTANT,
		OP_ADDRESS,
		OP_LABEL,
		OP_FUNCTION,
		OP_RELOP
	} kind;
	union{
		int value;
		char* name;
	}u;
}Operand;

#endif
