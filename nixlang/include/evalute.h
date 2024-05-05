#ifndef EVALUTE_H
#define EVALUTE_H


#include "synt.h"


enum Object_type {
	OBJECT_NUMBER,
	OBJECT_FLOAT_NUMBER,
	OBJECT_BOOLEAN,
	OBJECT_STRING,
	OBJECT_PATH,
	OBJECT_ARRAY,
	OBJECT_SET,
	OBJECT_FUNCTION,
	OBJECT_VARIABLE,
	OBJECT_VARIABLE_POINTER,
	OBJECT_OPERATION,
};


enum Op_type {
	OP_EQ,
	OP_NEQ,
	OP_LESS,
	OP_ELESS,
	OP_MORE,
	OP_EMORE,
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
	char value;
} Object_boolean;

typedef struct {
	char* value;
} Object_path;

typedef struct {
	char* value;
} Object_string;

typedef struct {
	char* argument_name;
	Object* body;
} Object_function;

typedef struct {
	char* name;
	Object* value;
} Object_variable;

typedef struct {
	char* name;
} Object_var_pointer;

typedef struct {
	enum Op_type type;
	unsigned int count;
	Object** args;
} Object_operation;


extern Object** variables;
extern unsigned int variables_count;

void init_def_vars();


Object* node_to_object(Node*);
Object* evalute(Object*);

void print_object(Object*, int);


#endif
