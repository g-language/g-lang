#include "lbolib.h"

enum {
    INFO,
    WARN,
    ERRR,
};

static void io_impl(int type, FILE* stream, const char* fmt, va_list va) {
    const char* s[] = {
        [INFO] = "> ",
        [WARN] = CLI_YELW "> ",
        [ERRR] = CLI_REDD "> ",
    };
    
    fprintf(stream, "%s" CLI_RSET, s[type]);
    vfprintf(stream, fmt, va);
    fputc('\n', stream);
    fflush(stream);
}

void info(const char* fmt, ...) {
    va_list va;
    
    va_start(va, fmt);
    io_impl(INFO, stdout, fmt, va);
    va_end(va);
}

void warn(const char* fmt, ...) {
    va_list va;
    
    va_start(va, fmt);
    io_impl(WARN, stderr, fmt, va);
    va_end(va);
}

void errr(const char* fmt, ...) {
    va_list va;
    
    va_start(va, fmt);
    io_impl(ERRR, stderr, fmt, va);
    va_end(va);
    
    exit(1);
}
