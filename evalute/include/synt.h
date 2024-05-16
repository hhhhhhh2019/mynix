#ifndef SYNT_H
#define SYNT_H


#include "node.h"
#include "lexer.h"
#include "utils.h"


typedef struct {
	char ok;
	Node* root;
	Malloc_info malloc_info;
} Synt_result;


Synt_result synt(Lexer_result);


#endif
