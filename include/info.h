#ifndef INFO_H
#define INFO_H

#include "types.h"

#define INFO_COLOR    0
#define WARNING_COLOR 1
#define ERROR_COLOR   2

extern const char* gPrintMsg[];

static inline void info(const char* fmt, ...) {
	char* msg;
	va_list va;
	
	va_start(va, fmt);
	vasprintf(&msg, fmt, va);
	va_end(va);
	
	fprintf(
		stdout,
		"%s%s\n" PRNT_RSET,
		gPrintMsg[INFO_COLOR],
		msg
	);
	
	free(msg);
}

static inline void warning(const char* fmt, ...) {
	char* msg;
	va_list va;
	
	va_start(va, fmt);
	vasprintf(&msg, fmt, va);
	va_end(va);
	
	fprintf(
		stderr,
		"%s%s\n" PRNT_RSET,
		gPrintMsg[WARNING_COLOR],
		msg
	);
	
	free(msg);
}

static inline void error(const char* fmt, ...) {
	char* msg;
	va_list va;
	
	va_start(va, fmt);
	vasprintf(&msg, fmt, va);
	va_end(va);
	
	fprintf(
		stderr,
		"%s%s\n" PRNT_RSET,
		gPrintMsg[ERROR_COLOR],
		msg
	);
	
	free(msg);
	
	exit(EXIT_FAILURE);
}

#endif