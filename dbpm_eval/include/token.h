#ifndef TOKEN_H
#define TOKEN_H


enum Token_type {
	NUMBER,
	FLOAT_NUMBER,
	STRING,
	LBR,
	RBR,
	LCBR,
	RCBR,
	LSBR,
	RSBR,
	COMMA,
	DOT,
	SEMICOLON,
	DOLLAR,
	PLUS,
	MINUS,
	STAR,
	SLASH,
	AMPERSAND,
	PIPE,
	CARET,
	EXCLAMATION,
	ASSIGN,
	EQUALS,
	NOT_EQUALS,
	LESS,
	LESS_EQUALS,
	MORE,
	MORE_EQUALS,
	INHERIT,
	IMPORT,
	IF,
	ELSE,
	THEN,
	LET,
	IN,
	SPACE,
	COMMENT,
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
