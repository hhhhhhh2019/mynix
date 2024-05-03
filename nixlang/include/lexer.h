#ifndef LEXER_H
#define LEXER_H


enum Token_type {
	SPACE,
	DEC_NUMBER,
	FLOAT_NUMBER,
	PATH,
	STRING,
	UNDEFINED,
	LET,
	IN,
	REC,
	WITH,
	INHERIT,
	IF,
	ELSE,
	THEN,
	TRUE,
	FALSE,
	SEMICOLON,
	COLON,
	DOLLAR,
	PLUS,
	MINUS,
	SLASH,
	STAR,
	AMPERSAND,
	PIPE,
	CARET,
	EXCLAMATION,
	DOT,
	COMMA,
	EPSILON,
	LBR,
	RBR,
	LCBR,
	RCBR,
	LSBR,
	RSBR,
	ASSIGN,
	EQUALS,
	NOT_EQUALS,
	LESS_EQUALS,
	MORE_EQUALS,
	TERMINALS_COUNT,
	Start,
	Set,
	Sargs,
	Sarg,
	Array,
	Aargs,
	E,
	E1,
	E2,
	E3,
	E4,
	E5,
	Name,
	Func_decl,
	Call,
};


typedef struct {
	enum Token_type type;
	char* value;
	char* filename;
	unsigned long line;
	unsigned long column;
} Token;


typedef struct {
	unsigned int tokens_count;
	Token* tokens;
} Lexer_result;


extern char* token_type_names[];

Lexer_result lexer(char* data, char* filename);


#endif
