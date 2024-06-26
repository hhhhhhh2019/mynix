#ifndef TOKENS_H
#define TOKENS_H


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
	IMPORT,
	IF,
	ELSE,
	THEN,
	SEMICOLON,
	COLON,
	EPSILON,
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
	Vdef,
	Vargs,
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
	Fargs,
	Farg,
	Array,
	Aarg,
	Set,
	Sargs,
	Sarg,
	Sid,
	Eval,
	Import,

	TOKEN_TYPES_COUNT,
};


typedef struct {
	enum Token_type type;
	char* value;
	char* filename;
	unsigned long line;
	unsigned long column;
} Token;

extern char* token_type_names[];


#endif
