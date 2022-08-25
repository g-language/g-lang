#ifndef STR_H
#define STR_H

#include "mem.h"

typedef enum regexflag {
	REGFLAG_START     = 1 << 24,
	REGFLAG_END       = 1 << 25,
	REGFLAG_COPY      = 1 << 26,
	REGFLAG_MATCH_NUM = 1 << 27,
	
	REGFLAG_FLAGMASK  = 0xFF000000,
	REGFLAG_NUMMASK   = 0x00FFFFFF,
} regexflag;

typedef struct strlist {
	char** item;
	u32    num;
} strlist;

void strlist_realloc(strlist* list, u32 num);
void strlist_free(strlist* list);
void strlist_add(strlist* list, const char* item);
void strlist_insert(strlist* list, const char* item, u32 slot);

char*regex(const char* str, const char* pattern, regexflag flag);

#endif