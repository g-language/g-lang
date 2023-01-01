#ifndef LBO_TYPES_H
#define LBO_TYPES_H
#ifndef LBO_LIB_H
#warning !
#endif

#include <stdlib.h>
#include <stdint.h>

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef float          f32;
typedef double         f64;
typedef _Bool          bool;
typedef uintptr_t      addr_t;

enum {
    false = 0,
    true  = 1,
};

struct file {
    const char* name;
    char*       data;
    size_t      size;
    size_t      msize;
    
    struct {
        bool throw;
    };
};

#endif