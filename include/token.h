#ifndef TOKEN_H
#define TOKEN_H

#include "macros.h"
#include "types.h"

enum PACKED token_type
{
	TOKEN_WHITESPACE
	, TOKEN_NEWLINE
	, TOKEN_VARIABLE
	, TOKEN_FUNCTION
	, TOKEN_COMMA
	, TOKEN_OPEN_BRACE
	, TOKEN_CLOSE_BRACE
	, TOKEN_OPEN_CURLY_BRACE
	, TOKEN_CLOSE_CURLY_BRACE
	, TOKEN_TYPE
	, TOKEN_KEYWORD
};

struct token_list {
	enum token_type type;
	u8              wow;
};

#endif