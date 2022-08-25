#ifndef TOKEN_H
#define TOKEN_H

#include "str.h"

typedef enum tokentype {
	TOKEN_VARIABLE,
	TOKEN_FUNCTION,
	TOKEN_COMMA,
	TOKEN_OPEN_BRACE,
	TOKEN_CLOSE_BRACE,
	TOKEN_OPEN_CURLY_BRACE,
	TOKEN_CLOSE_CURLY_BRACE,
	TOKEN_TYPE,
	TOKEN_KEYWORD,
	TOKEN_PREPROCESSOR,
	TOKEN_OPERATOR,
	TOKEN_NUMBER,
} tokentype;

typedef struct token {
	struct token* next;
	tokentype     type;
	const char*   word;
	u32 indent;
	u32 linenum;
	u32 charnum;
	struct {
		bool firstword : 1;
	};
} token;

token* tokenize(const char* str);

#endif