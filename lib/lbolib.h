#ifndef LBO_LIB_H
#define LBO_LIB_H

#ifdef FOOL_THE_IDE
#define free lib_free
#endif

#include <stdio.h>
#include <stdlib.h>
#undef free
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "lbo/macros.h"
#include "lbo/types.h"

#ifndef FOOL_THE_IDE
#define strncat(str, cat, size) strncat(str, cat, (size) - 1)
#define strncpy(str, cpy, size) strncpy(str, cpy, (size) - 1)
#endif

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

void info(const char* fmt, ...);
void warn(const char* fmt, ...);
void errr(const char* fmt, ...);

void* x_alloc(size_t z);
char* x_strdup(const char* s);
char* x_strndup(const char* s, size_t n);
char* x_memdup(const char* d, size_t n);
char* x_fmt(const char* fmt, ...);
char* x_rep(const char* s, const char* a, const char* b);
char* x_cpyline(const char* s, size_t i);
char* x_cpyword(const char* s, size_t i);
char* x_path(const char* s);
char* x_basename(const char* s);
char* x_filename(const char* s);
char* x_randstr(size_t size, const char* charset);
char* x_strunq(const char* s);
char* x_enumify(const char* s);
char* x_canitize(const char* s);

char* strdup(const char* s);
char* strndup(const char* s, size_t n);
char* memdup(const char* d, size_t n);
char* fmt(const char* fmt, ...);
char* rep(const char* s, const char* a, const char* b);
char* cpyline(const char* s, size_t i);
char* cpyword(const char* s, size_t i);
char* path(const char* s);
char* basename(const char* s);
char* filename(const char* s);
char* randstr(size_t size, const char* charset);
char* strunq(const char* s);
char* enumify(const char* s);
char* canitize(const char* s);

char* strwstr(const char*, const char*);
int strrep(char*, const char*, const char*);
int strwrep(char*, const char*, const char*);
char* strline(const char* str, size_t i);
char* strword(const char* str, size_t i);
size_t linelen(const char* str);
size_t wordlen(const char* str);

char* strcnv8(char* dst, const short* src);
short* strcnv16(short* dst, const char* src);
size_t strlen16(const short* s);

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

struct file file_new(bool throw, size_t allosize);
void file_realloc(struct file* this, size_t sz);
bool file_load(struct file* this, const char* file, char type);
void file_free(struct file* this);

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#ifdef FOOL_THE_IDE

void log(const char* fmt, ...);
#else // FOOL_THE_IDE

void __log(const char* func, uint cpyline, const char* fmt, ...);
#define log(...) __log(__FUNCTION__, __LINE__, __VA_ARGS__)
#endif // FOOL_THE_IDE

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#ifdef FOOL_THE_IDE

int clamp_min(int v, int min);
int clamp_max(int v, int max);
int clamp(int v, int min, int max);

int max(int a, int b);
int min(int a, int b);

void assert(bool condition);
#else // FOOL_THE_IDE

#define clamp_min(value, min)  ((value) < (min) ? (min) : (value))
#define clamp_max(value, max)  ((value) > (max) ? (max) : (value))
#define clamp(value, min, max) (clamp_min(clamp_max(value, max), min))

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define assert(condition) do {                                       \
        if (!(condition)) {                                          \
            errr("%s::%d (%s)", __FUNCTION__, __LINE__, #condition); \
        } } while (0)
#endif // FOOL_THE_IDE

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

void* mem_new(const size_t);

#ifndef FOOL_THE_IDE
void* mem_free(const void*);

#define __EXT_FREE_METHOD(a) a = mem_free(a);
#define free(...)            ({                      \
        VA_ARG_MANIP(__EXT_FREE_METHOD, __VA_ARGS__) \
        NULL;                                        \
    })

#define fswap(f, s) do { mem_free(f); f = s; } while (0)
#else // FOOL_THE_IDE

void fswap(const void* f, const void* swap);
void* free(const void*, ...);
#endif // FOOL_THE_IDE

#define new(type)  mem_new(sizeof(type))
#define newz(size) mem_new(size)

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#include "lbo/static.h"

#endif