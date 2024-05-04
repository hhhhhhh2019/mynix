#ifndef EVALUTE_H
#define EVALUTE_H


#include "synt.h"


enum Object_type {
	OBJECT_NUMBER,
	OBJECT_FLOAT_NUMBER,
	OBJECT_STRING,
	OBJECT_PATH,
	OBJECT_ARRAY,
	OBJECT_SET,
	OBJECT_FUNCTION,
	OBJECT_VARIABLE,
	OBJECT_OPERATION,
};


enum Op_type {
	OP_SUM,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_AND,
	OP_OR,
	OP_XOR,
	OP_NOT,
	OP_DOT,
};


extern char* object_type_names[];
extern char* operation_type_names[];



typedef struct {
	enum Object_type type;
	void* data;
} Object;

typedef struct {
	long value;
} Object_number;

typedef struct {
	double value;
} Object_float;

typedef struct {
	char* value;
} Object_path;

typedef struct {
	char* value;
} Object_string;

typedef struct {
	Object* argument;
	Object* body;
} Object_function;

typedef struct {
	char* name;
	Object* value;
} Object_variable;

typedef struct {
	enum Op_type type;
	unsigned int count;
	Object** args;
} Object_operation;


Object* node_to_object(Node*);
Object* evalute(Object*);

void print_object(Object*, int);


#endif
