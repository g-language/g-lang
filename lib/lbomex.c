#include "lbolib.h"
#include <pthread.h>

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

typedef pthread_mutex_t mutex_t;

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

static struct {
    char*       head;
    size_t      offset;
    size_t      max;
    size_t      f;
    const char* header;
} g_bufx = {
    .max    = 8000000,
    .f      =  100000,
    
    .header = "xbuf\xDE\xAD\xBE\xEF"
};

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

void* x_alloc(size_t size) {
    static mutex_t xmutex;
    char* ret;
    
    if (size <= 0)
        return NULL;
    
    // if (size >= g_bufx.f)
    //     printf_error("Biggest Failure");
    
    pthread_mutex_lock(&xmutex);
    if (g_bufx.head == NULL)
        g_bufx.head = newz(g_bufx.max);
    
    if (g_bufx.offset + size + 10 >= g_bufx.max)
        g_bufx.offset = 0;
    
    // Write Header
    ret = &g_bufx.head[g_bufx.offset];
    memcpy(ret, g_bufx.header, 8);
    g_bufx.offset += 8;
    
    // Write Data
    ret = &g_bufx.head[g_bufx.offset];
    g_bufx.offset += size + 1;
    pthread_mutex_unlock(&xmutex);
    
    return memset(ret, 0, size + 1);
}

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

char* __impl_memdup(void* (*falloc)(size_t), const char* data, size_t size) {
    if (!data || !size)
        return NULL;
    
    return memcpy(falloc(size), data, size);
}

char* __impl_strdup(void* (*falloc)(size_t), const char* s) {
    return __impl_memdup(falloc, s, strlen(s) + 1);
}

char* __impl_strndup(void* (*falloc)(size_t), const char* s, size_t n) {
    if (!n || !s) return NULL;
    size_t csz = strnlen(s, n);
    char* new = falloc(n + 1);
    char* res = memcpy (new, s, csz);
    
    return res;
}

char* __impl_fmt(void* (*falloc)(size_t), const char* fmt, va_list va) {
    char buf[8192];
    int len;
    
    len = vsnprintf(buf, 8192, fmt, va);
    
    return __impl_memdup(falloc, buf, len + 1);
}

char* __impl_rep(void* (*falloc)(size_t), const char* s, const char* a, const char* b) {
    char* r = falloc(strlen(s) * 4 + strlen(b) * 8);
    
    strcpy(r, s);
    strrep(r, a, b);
    
    return r;
}

char* __impl_cpyline(void* (*falloc)(size_t), const char* s, size_t cpyline) {
    if (!(s = strline(s, cpyline))) return NULL;
    
    return __impl_strndup(falloc, s, linelen(s));
}

char* __impl_cpyword(void* (*falloc)(size_t), const char* s, size_t word) {
    if (!(s = strword(s, word))) return NULL;
    
    return __impl_strndup(falloc, s, wordlen(s));
}

char* __impl_path(void* (*falloc)(size_t), const char* s) {
    char* buffer;
    int point;
    int slash;
    
    if (s == NULL)
        return NULL;
    
    nested(void, slash_n_point, (const char* src, int* slash, int* point)) {
        int strSize = strlen(src);
        
        *slash = 0;
        *point = 0;
        
        for (int i = strSize; i > 0; i--) {
            if (*point == 0 && src[i] == '.') {
                *point = i;
            }
            if (src[i] == '/' || src[i] == '\\') {
                *slash = i;
                break;
            }
        }
    };
    
    slash_n_point(s, &slash, &point);
    
    if (slash == 0)
        slash = -1;
    
    buffer = falloc(slash + 1 + 1);
    memcpy(buffer, s, slash + 1);
    buffer[slash + 1] = '\0';
    
    return buffer;
}

char* __impl_basename(void* (*falloc)(size_t), const char* s) {
    char* la = strrchr(s, '/');
    char* lb = strrchr(s, '\\');
    char* ls = (char*)min((addr_t)la, (addr_t)lb);
    
    if (!ls++)
        ls = (char*)s;
    
    return __impl_strndup(falloc, ls, strcspn(ls, "."));
}

char* __impl_filename(void* (*falloc)(size_t), const char* s) {
    char* la = strrchr(s, '/');
    char* lb = strrchr(s, '\\');
    char* ls = (char*)min((addr_t)la, (addr_t)lb);
    
    if (!ls++)
        ls = (char*)s;
    
    return __impl_strdup(falloc, ls);
}

char* __impl_randstr(void* (*falloc)(size_t), size_t size, const char* charset) {
    const char defcharset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const char* set = charset ? charset : defcharset;
    const int setz = (charset ? strlen(charset) : sizeof(defcharset)) - 1;
    char* str = falloc(size);
    
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % setz;
            str[n] = set[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* __impl_strunq(void* (*falloc)(size_t), const char* s) {
    char* new = __impl_strdup(falloc, s);
    
    strrep(new, "\"", "");
    strrep(new, "'", "");
    
    return new;
}

static char* ifyize(char* new, const char* str, int (*tocase)(int)) {
    size_t w = 0;
    const size_t len = strlen(str);
    
    enum {
        NONE = 0,
        IS_LOWER,
        IS_UPPER,
        NONALNUM,
    } prev = NONE, this = NONE;
    
    for (size_t i = 0; i < len; i++) {
        const char chr = str[i];
        
        if (isalnum(chr)) {
            
            if (isupper(chr))
                this = IS_UPPER;
            else
                this = IS_LOWER;
            
            if (this == IS_UPPER && prev == IS_LOWER)
                new[w++] = '_';
            else if (prev == NONALNUM)
                new[w++] = '_';
            new[w++] = tocase(chr);
            
        } else
            this = NONALNUM;
        
        prev = this;
    }
    
    return new;
}

char* __impl_enumify(void* (*falloc)(size_t), const char* s) {
    char* new = falloc(strlen(s) * 2);
    
    return ifyize(new, s, toupper);
}

char* __impl_canitize(void* (*falloc)(size_t), const char* s) {
    char* new = falloc(strlen(s) * 2);
    
    return ifyize(new, s, tolower);
}

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

char* x_strdup(const char* s) {
    return __impl_strdup(x_alloc, s);
};

char* x_strndup(const char* s, size_t n) {
    return __impl_strndup(x_alloc, s, n);
};

char* x_memdup(const char* d, size_t n) {
    return __impl_memdup(x_alloc, d, n);
};

char* x_fmt(const char* fmt, ...) {
    va_list va; va_start(va, fmt);
    char* r = __impl_fmt(x_alloc, fmt, va);
    
    va_end(va);
    return r;
};

char* x_rep(const char* s, const char* a, const char* b) {
    return __impl_rep(x_alloc, s, a, b);
};

char* x_cpyline(const char* s, size_t i) {
    return __impl_cpyline(x_alloc, s, i);
};

char* x_cpyword(const char* s, size_t i) {
    return __impl_cpyword(x_alloc, s, i);
};

char* x_path(const char* s) {
    return __impl_path(x_alloc, s);
};

char* x_basename(const char* s) {
    return __impl_basename(x_alloc, s);
};

char* x_filename(const char* s) {
    return __impl_filename(x_alloc, s);
};

char* x_randstr(size_t size, const char* charset) {
    return __impl_randstr(x_alloc, size, charset);
};

char* x_strunq(const char* s) {
    return __impl_strunq(x_alloc, s);
};

char* x_enumify(const char* s) {
    return __impl_enumify(x_alloc, s);
};

char* x_canitize(const char* s) {
    return __impl_canitize(x_alloc, s);
};

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

char* strdup(const char* s) {
    return __impl_strdup(mem_new, s);
};

char* strndup(const char* s, size_t n) {
    return __impl_strndup(mem_new, s, n);
};

char* memdup(const char* d, size_t n) {
    return __impl_memdup(mem_new, d, n);
};

char* fmt(const char* fmt, ...) {
    va_list va; va_start(va, fmt);
    char* r = __impl_fmt(mem_new, fmt, va);
    
    va_end(va);
    return r;
};

char* rep(const char* s, const char* a, const char* b) {
    return __impl_rep(mem_new, s, a, b);
};

char* cpyline(const char* s, size_t i) {
    return __impl_cpyline(mem_new, s, i);
};

char* cpyword(const char* s, size_t i) {
    return __impl_cpyword(mem_new, s, i);
};

char* path(const char* s) {
    return __impl_path(mem_new, s);
};

char* basename(const char* s) {
    return __impl_basename(mem_new, s);
};

char* filename(const char* s) {
    return __impl_filename(mem_new, s);
};

char* randstr(size_t size, const char* charset) {
    return __impl_randstr(mem_new, size, charset);
};

char* strunq(const char* s) {
    return __impl_strunq(mem_new, s);
};

char* enumify(const char* s) {
    return __impl_enumify(mem_new, s);
};

char* canitize(const char* s) {
    return __impl_canitize(mem_new, s);
};

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
