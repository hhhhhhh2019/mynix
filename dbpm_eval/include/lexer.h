#ifndef LEXER_H
#define LEXER_H


#include "token.h"
#include "utils.h"


typedef struct {
	char ok;
	unsigned int tokens_count;
	Token* tokens;
	Malloc_info malloc_info;
} Lexer_result;


Lexer_result lexer(char* data, char* filename);
void lexer_cleanup(Lexer_result);


#endif
