#ifndef EVALUTE_H
#define EVALUTE_H


#include "object.h"


struct Scope;
typedef struct Scope {
	struct Scope* parent;
	Object_variable* variables;
	unsigned int variables_count;
} Scope;

extern Scope* current_scope;

void init_default_variables();

Object* evalute(Object*);


#endif
