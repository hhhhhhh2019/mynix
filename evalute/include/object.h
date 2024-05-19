#ifndef OBJECT_H
#define OBJECT_H


#include "node.h"


enum Object_type {
	OBJECT_NUMBER,
	OBJECT_FLOAT_NUMBER,
	OBJECT_BOOLEAN,
	OBJECT_STRING,
	OBJECT_PATH,
	OBJECT_ARRAY,
	OBJECT_SET,
	OBJECT_FUNCTION,
	OBJECT_FUNCTION_SET,
	OBJECT_FUNCTION_EXTERNAL,
	OBJECT_VARIABLE,
	OBJECT_NAME,
	OBJECT_OPERATION,
};

enum Op_type {
	OP_EQ,
	OP_NEQ,
	OP_LESS,
	OP_ELESS,
	OP_MORE,
	OP_EMORE,
	OP_ASSIGN,
	OP_SUM,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_AND,
	OP_OR,
	OP_XOR,
	OP_NOT,
	OP_DOT,
	OP_CALL,
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
	Object* body;
	char allow_other;
	int args_count;
	char** args_names;
} Object_function_set;

typedef struct {
	Object* (*body) (Object*);
} Object_function_external;

typedef struct {
	unsigned int count;
	Object** elems;
} Object_array;

typedef struct {
	char* name;
	Object* value;
} Set_arg;

typedef struct {
	unsigned int count;
	Set_arg* elems;
} Object_set;

typedef struct {
	char* name;
	Object* value;
} Object_variable;

typedef struct {
	char* name;
} Object_name;

typedef struct {
	enum Op_type type;
	unsigned int count;
	Object** args;
} Object_operation;


Object* node_to_object(Node*);


#endif
