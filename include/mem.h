#ifndef MEM_H
#define MEM_H

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "types.h"

// post_free queue
void *pque(const void *data);
// post_free free all
void pfree(void);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static inline void *zalloc(size_t size)
{
	return memset(malloc(size), 0, size);
}

static inline void *memdup(const void *data, size_t size)
{
	return memcpy(malloc(size), data, size);
}

static inline char *line(const char *str, s32 line)
{
	const char *ln = str;
	
	if (!str)
		return NULL;
	
	while (line--)
	{
		ln = strpbrk(ln, "\n\r");
		
		if (!ln++)
			return NULL;
	}
	
	return (char *)ln;
}

static inline char *word(const char *str, s32 word)
{
	if (!str)
		return NULL;
	
	while (!isgraph(*str))
		str++;
	while (word--)
	{
		if (!(str = strpbrk(str, " \t\n\r")))
			return NULL;
		while (!isgraph(*str))
			str++;
	}
	
	return (char *)str;
}

static inline size_t linecount(const char *str)
{
	size_t count = 1;
	
	if (!str)
		return 0;
	
	while (str)
	{
		str = line(str, 1);
		count++;
	}
	
	return count;
}

static inline char *wrdcpy(const char *str)
{
	return strndup(str, strcspn(str, " \t"));
}

static inline char *linecpy(const char *str)
{
	return strndup(str, strcspn(str, "\n\r"));
}

#pragma GCC diagnostic pop

#endif