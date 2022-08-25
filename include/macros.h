#ifndef MACRO_H
#define MACRO_H

#define _GNU_SOURCE
#define __CRT__NO_INLINE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define var __auto_type
#define PACKED __attribute__((packed))

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

#define nodeadd(head, node) do { \
		typeof(node) * n = &head; \
		while (*n) n = &(*n)->next; \
		*n = node; \
} while (0)

#define noderem(head, node) do { \
		typeof(node) * n = &head; \
		while (*n != node) n = &(*n)->next; \
		*n = node->next; \
} while (0)

#define nodefree(head, node) do { \
		typeof(node) killNode = node; \
		noderem(head, node); \
		free(killNode); \
} while (0)

#endif