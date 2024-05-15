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
	WITH,
	INHERIT,
	IF,
	ELSE,
	THEN,
	// TRUE,
	// FALSE,
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
	LBR,
	RBR,
	LCBR,
	RCBR,
	LSBR,
	RSBR,
	ASSIGN,
	EQUALS,
	NOT_EQUALS,
	LESS,
	LESS_EQUALS,
	MORE,
	MORE_EQUALS,

	TERMINALS_COUNT,

	EOI,
	E,
	E1,
	E2,
	E3,
	E4,
	E5,
	E6,
	Name,
	Call,
	Func_decl,
	Array,
	Aarg,
	Set,
	Sargs,
	Sarg,
	Sid,
	Eval,
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
