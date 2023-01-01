#ifdef FOOL_THE_IDE
#undef _WIN32
#endif

#include "lbolib.h"
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ftw.h>
#include <pthread.h>
#ifdef _WIN32
#include <windows.h>
#include <libloaderapi.h>
#endif

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

typedef pthread_mutex_t mutex_t;

#define construct __attribute__ ((constructor)) void
#define destruct  __attribute__ ((destructor)) void

#define BUFFER buf[1024]

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

char* strwstr(const char* haystack, const char* needle) {
    char* p = strstr(haystack, needle);
    
    while (p) {
        if (!isgraph(p[-1]) && !isgraph(p[strlen(needle)]))
            return p;
        
        p = strstr(p + 1, needle);
    }
    
    return NULL;
}

int strrep(char* src, const char* word, const char* replacement) {
    size_t repLen = strlen(replacement);
    size_t wordLen = strlen(word);
    int diff = 0;
    char* ptr;
    
    ptr = strstr(src, word);
    
    while (ptr != NULL) {
        size_t remLen = strlen(ptr + wordLen);
        memmove(ptr + repLen, ptr + wordLen, remLen + 1);
        memcpy(ptr, replacement, repLen);
        ptr = strstr(ptr + repLen, word);
        diff = true;
    }
    
    return diff;
}

int strwrep(char* src, const char* word, const char* replacement) {
    size_t repLen = strlen(replacement);
    size_t wordLen = strlen(word);
    int diff = 0;
    char* ptr;
    
    ptr = strwstr(src, word);
    
    while (ptr != NULL) {
        size_t remLen = strlen(ptr + wordLen);
        memmove(ptr + repLen, ptr + wordLen, remLen + 1);
        memcpy(ptr, replacement, repLen);
        ptr = strwstr(ptr  + repLen, word);
        diff = true;
    }
    
    return diff;
}

char* strline(const char* str, size_t line) {
    const char* ln = str;
    
    if (!str)
        return NULL;
    
    if (line) {
        if (line > 0) {
            while (line--) {
                ln = strchr(ln, '\n');
                
                if (!ln++)
                    return NULL;
                
                if (*ln == '\r')
                    ln++;
            }
        } else {
            while (line++) {
                ln = strrchr(ln, '\n');
                
                if (!ln--)
                    return NULL;
            }
            
            if (ln)
                ln += 2;
        }
    }
    
    return (char*)ln;
}

char* strword(const char* str, size_t word) {
    if (!str)
        return NULL;
    
    while (!isgraph(*str)) str++;
    while (word--) {
        if (!(str = strpbrk(str, " \t\n\r"))) return NULL;
        while (!isgraph(*str)) str++;
    }
    
    return (char*)str;
}

size_t linelen(const char* str) {
    return strcspn(str, "\n\r");
}

size_t wordlen(const char* str) {
    return strcspn(str, " \t\n\r");
}

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// https://github.com/Davipb/utf8-utf16-converter

typedef enum {
    BMP_END                          = 0xFFFF,
    UNICODE_MAX                      = 0x10FFFF,
    INVALID_CODEPOINT                = 0xFFFD,
    GENERIC_SURROGATE_VALUE          = 0xD800,
    GENERIC_SURROGATE_MASK           = 0xF800,
    HIGH_SURROGATE_VALUE             = 0xD800,
    LOW_SURROGATE_VALUE              = 0xDC00,
    SURROGATE_MASK                   = 0xFC00,
    SURROGATE_CODEPOINT_OFFSET       = 0x10000,
    SURROGATE_CODEPOINT_MASK         = 0x03FF,
    SURROGATE_CODEPOINT_BITS         = 10,
    UTF8_1_MAX                       = 0x7F,
    UTF8_2_MAX                       = 0x7FF,
    UTF8_3_MAX                       = 0xFFFF,
    UTF8_4_MAX                       = 0x10FFFF,
    UTF8_CONTINUATION_VALUE          = 0x80,
    UTF8_CONTINUATION_MASK           = 0xC0,
    UTF8_CONTINUATION_CODEPOINT_BITS = 6,
    UTF8_LEADING_BYTES_LEN           = 4,
} __utf8_define_t;

typedef struct {
    uchar mask;
    uchar value;
} utf8_pattern;

static const utf8_pattern utf8_leading_bytes[] = {
    { 0x80, 0x00 }, // 0xxxxxxx
    { 0xE0, 0xC0 }, // 110xxxxx
    { 0xF0, 0xE0 }, // 1110xxxx
    { 0xF8, 0xF0 }, // 11110xxx
};

static size_t wccv_lenchar(uint codepoint) {
    if (codepoint <= UTF8_1_MAX)
        return 1;
    
    if (codepoint <= UTF8_2_MAX)
        return 2;
    
    if (codepoint <= UTF8_3_MAX)
        return 3;
    
    return 4;
}

static size_t wccv_encchar(uint codepoint, char* utf8, size_t index) {
    int size = wccv_lenchar(codepoint);
    
    // Write the continuation bytes in reverse order first
    for (int cont_index = size - 1; cont_index > 0; cont_index--) {
        uchar cont = codepoint & ~UTF8_CONTINUATION_MASK;
        cont |= UTF8_CONTINUATION_VALUE;
        
        utf8[index + cont_index] = cont;
        codepoint >>= UTF8_CONTINUATION_CODEPOINT_BITS;
    }
    
    utf8_pattern pattern = utf8_leading_bytes[size - 1];
    
    uchar lead = codepoint & ~(pattern.mask);
    
    lead |= pattern.value;
    
    utf8[index] = lead;
    
    return size;
}

static size_t wccv_encshort(uint codepoint, short* utf16, size_t index) {
    if (codepoint <= BMP_END) {
        utf16[index] = codepoint;
        
        return 1;
    }
    
    codepoint -= SURROGATE_CODEPOINT_OFFSET;
    
    ushort low = LOW_SURROGATE_VALUE;
    low |= codepoint & SURROGATE_CODEPOINT_MASK;
    
    codepoint >>= SURROGATE_CODEPOINT_BITS;
    
    ushort high = HIGH_SURROGATE_VALUE;
    high |= codepoint & SURROGATE_CODEPOINT_MASK;
    
    utf16[index] = high;
    utf16[index + 1] = low;
    
    return 2;
}

static uint wccv_decchar(const char* utf8, size_t len, size_t* index) {
    uchar leading = utf8[*index];
    int encoding_len = 0;
    utf8_pattern leading_pattern;
    bool matches = false;
    
    do {
        encoding_len++;
        leading_pattern = utf8_leading_bytes[encoding_len - 1];
        
        matches = (leading & leading_pattern.mask) == leading_pattern.value;
        
    } while (!matches && encoding_len < UTF8_LEADING_BYTES_LEN);
    
    if (!matches)
        return INVALID_CODEPOINT;
    
    uint codepoint = leading & ~leading_pattern.mask;
    
    for (int i = 0; i < encoding_len - 1; i++) {
        if (*index + 1 >= len)
            return INVALID_CODEPOINT;
        
        uchar continuation = utf8[*index + 1];
        if ((continuation & UTF8_CONTINUATION_MASK) != UTF8_CONTINUATION_VALUE)
            return INVALID_CODEPOINT;
        
        codepoint <<= UTF8_CONTINUATION_CODEPOINT_BITS;
        codepoint |= continuation & ~UTF8_CONTINUATION_MASK;
        
        (*index)++;
    }
    
    int proper_len = wccv_lenchar(codepoint);
    
    if (proper_len != encoding_len)
        return INVALID_CODEPOINT;
    if (codepoint < BMP_END && (codepoint & GENERIC_SURROGATE_MASK) == GENERIC_SURROGATE_VALUE)
        return INVALID_CODEPOINT;
    if (codepoint > UNICODE_MAX)
        return INVALID_CODEPOINT;
    
    return codepoint;
}

static uint wccv_decshort(const short* utf16, size_t len, size_t* index) {
    ushort high = utf16[*index];
    
    if ((high & GENERIC_SURROGATE_MASK) != GENERIC_SURROGATE_VALUE)
        return high;
    if ((high & SURROGATE_MASK) != HIGH_SURROGATE_VALUE)
        return INVALID_CODEPOINT;
    if (*index == len - 1)
        return INVALID_CODEPOINT;
    
    ushort low = utf16[*index + 1];
    
    if ((low & SURROGATE_MASK) != LOW_SURROGATE_VALUE)
        return INVALID_CODEPOINT;
    (*index)++;
    uint result = high & SURROGATE_CODEPOINT_MASK;
    
    result <<= SURROGATE_CODEPOINT_BITS;
    result |= low & SURROGATE_CODEPOINT_MASK;
    result += SURROGATE_CODEPOINT_OFFSET;
    
    return result;
}

char* strcnv8(char* dst, const short* src) {
    size_t dstIndex = 0;
    size_t len = strlen16(src) + 1;
    
    if (!dst)
        dst = x_alloc(len);
    
    for (size_t indexSrc = 0; indexSrc < len; indexSrc++)
        dstIndex += wccv_encchar(wccv_decshort(src, len, &indexSrc), dst, dstIndex);
    
    return dst;
}

short* strcnv16(short* dst, const char* src) {
    size_t dstIndex = 0;
    size_t len = strlen(src) + 1;
    
    if (!dst)
        dst = x_alloc(len * 3);
    
    for (size_t srcIndex = 0; srcIndex < len; srcIndex++)
        dstIndex += wccv_encshort(wccv_decchar(src, len, &srcIndex), dst, dstIndex);
    
    return dst;
}

size_t strlen16(const short* s) {
    size_t len = 0;
    
    while (s[len] != 0)
        len++;
    
    return len;
}

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

time_t sys_stat(const char* f) {
    struct stat s = {};
    
    if (stat(f, &s) == -1)
        return 0;
    
    return max(s.st_mtime, s.st_ctime);
}

bool sys_isdir(const char* f) {
    struct stat st = { 0 };
    
    if (stat(f, &st) == -1)
        return false;
    if (S_ISDIR(st.st_mode))
        return true;
    
    return false;
}

const char* sys_app(void) {
    static char BUFFER;
    
    if (!buf[0]) {
        int x;
#ifdef _WIN32
        x = GetModuleFileName(NULL, buf, 512);
#else
        x = readlink("/proc/self/exe", buf, 512);
#endif
        log("app%d:" CLI_YELW "%s", x, buf);
    }
    
    return buf;
}

bool sys_mkdir(const char* f) {
    f = x_path(f);
    
    return true;
}

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#define stdlog            stderr
#define FAULT_BUFFER_SIZE (1024)
#define FAULT_LOG_NUM     12

static struct {
    char* msg[FAULT_LOG_NUM];
    char* func[FAULT_LOG_NUM];
    int   line[FAULT_LOG_NUM];
    bool  init;
} s_log;

static void log_signal_title(int arg, FILE* file) {
    const char* errorMsg[] = {
        "\a0",
        "\a1 - Hang Up",
        "\a2 - Interrupted",   // SIGINT
        "\a3 - Quit",
        "\a4 - Illegal Instruction",
        "\a5 - Trap",
        "\a6 - Abort()",
        "\a7 - Illegal Memory Access",
        "\a8 - Floating Point Exception",
        "\a9 - Killed",
        "\a10 - Programmer Error",
        "\a11 - Segmentation Fault",
        "\a12 - Programmer Error",
        "\a13 - Pipe Death",
        "\a14 - Alarm",
        "\a15 - Killed",
        
        "\aLog List",
    };
    
    if (arg != 0xDEADBEEF)
        fprintf(file, "" CLI_GRAY "[ " CLI_REDD "%s " CLI_GRAY "]\n", errorMsg[clamp_max(arg, 16)]);
    else
        fprintf(file, "" CLI_GRAY "[ " CLI_REDD "LOG " CLI_GRAY "]\n");
    fprintf(file, "\n");
}

static void log_signal_info(int arg, FILE* file) {
    
    for (int i = FAULT_LOG_NUM - 1, j = 0; i >= 0; i--, j++) {
        char* pfunc = j == 0 ? "__log_none__" : s_log.func[i + 1];
        char fmt[16];
        
        snprintf(fmt, 16, "%d:", s_log.line[i]);
        if (strcmp(s_log.func[i], pfunc))
            fprintf(file, "" CLI_YELW "%s" CLI_GRAY "();\n", s_log.func[i]);
        fprintf(file, "" CLI_GRAY "%-8s" CLI_RSET "%s\n", fmt, s_log.msg[i]);
    }
    
    if (arg == 16)
        fprintf(file, "\n");
}

static void log_signal(int arg) {
    static volatile bool ran = 0;
    
    if (!s_log.init)
        return;
    if (ran) return;
    ran = true;
    s_log.init = false;
    
    log_signal_title(arg, stdlog);
    log_signal_info(arg, stdlog);
    
    if (arg == 0xDEADBEEF) {
        ran = false;
        s_log.init = true;
        
        return;
    }
    
    exit(arg);
}

static void log_init() {
    if (s_log.init)
        return;
    for (int i = 1; i < 16; i++)
        signal(i, log_signal);
    for (int i = 0; i < FAULT_LOG_NUM; i++) {
        s_log.msg[i] = newz(FAULT_BUFFER_SIZE);
        s_log.func[i] = newz(128);
    }
    
    s_log.init = true;
}

static void log_free() {
    if (!s_log.init)
        return;
    s_log.init = 0;
    for (int i = 0; i < FAULT_LOG_NUM; i++) {
        free(s_log.msg[i]);
        free(s_log.func[i]);
    }
}

void __log(const char* func, uint line, const char* txt, ...) {
    if (!s_log.init)
        return;
    if (s_log.msg[0] == NULL)
        return;
    
    arrmove_r(s_log.msg, 0, FAULT_LOG_NUM);
    arrmove_r(s_log.func, 0, FAULT_LOG_NUM);
    arrmove_r(s_log.line, 0, FAULT_LOG_NUM);
    
    va_list args;
    va_start(args, txt);
    vsnprintf(s_log.msg[0], FAULT_BUFFER_SIZE, txt, args);
    va_end(args);
    
    strcpy(s_log.func[0], func);
    s_log.line[0] = line;
}

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

construct LBO_INIT(void) {
    log_init();
    
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    system("");
#endif
}

destruct LBO_DEST(void) {
    log_free();
}
