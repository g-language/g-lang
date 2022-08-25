#ifndef TOKEN_H
#define TOKEN_H

#include "mem.h"

typedef enum token_type
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
} token_type;

typedef struct tknlist
{
	struct tknlist  *next;
	enum token_type type;
	u8              wow;
} tknlist;

typedef struct strlist
{
	char **item;
	u32  num;
} strlist;

void strlist_realloc(strlist *list, u32 num);
void strlist_free(strlist *list);
void strlist_add(strlist *list, const char *item);
void strlist_insert(strlist *list, const char *item, u32 slot);

#endif