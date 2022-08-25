#ifndef MACRO_H
#define MACRO_H

#define _GNU_SOURCE
#define __CRT__NO_INLINE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>

#define var    __auto_type
#define PACKED __attribute__((packed))

#define PRNT_DGRY "\e[90;2m"
#define PRNT_DRED "\e[91;2m"
#define PRNT_GRAY "\e[90m"
#define PRNT_REDD "\e[91m"
#define PRNT_GREN "\e[92m"
#define PRNT_YELW "\e[93m"
#define PRNT_BLUE "\e[94m"
#define PRNT_PRPL "\e[95m"
#define PRNT_CYAN "\e[96m"
#define PRNT_DARK "\e[2m"
#define PRNT_ITLN "\e[3m"
#define PRNT_UNDL "\e[4m"
#define PRNT_BLNK "\e[5m"
#define PRNT_RSET "\e[m"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define new(type) zalloc(sizeof(type))

#define array_count(arr) (u32)(sizeof(arr) / sizeof(arr[0]))

#define free(data) do { \
		if (data) free(data); \
		data = NULL; \
} while (0)

#define arrmove_r(arr, start, count) do { \
		var v = (arr)[(start) + (count) - 1]; \
		for (int i = (count) + (start) - 1; i > (start); i--) \
		(arr)[i] = (arr)[i - 1]; \
		(arr)[(start)] = v; \
} while (0)

#define arrmove_l(arr, start, count) do { \
		var v = (arr)[(start)]; \
		for (int i = (start); i < (count) + (start); i++) \
		(arr)[i] = (arr)[i + 1]; \
		(arr)[(count) + (start) - 1] = v; \
} while (0)

#define node_add(head, node) do { \
		typeof(node) * n = &head; \
		while (*n) n = &(*n)->next; \
		*n = node; \
} while (0)

#define node_rem(head, node) do { \
		typeof(node) * n = &head; \
		while (*n != node) n = &(*n)->next; \
		*n = node->next; \
} while (0)

#define node_free(head, node) do { \
		typeof(node) killNode = node; \
		node_rem(head, node); \
		free(killNode); \
} while (0)

#endif