
#ifndef LBO_MACROS_H
#define LBO_MACROS_H
#ifndef LBO_LIB_H
#warning !
#endif

#define var __auto_type

#define swap(a, b) do { \
        var t = a;      \
        a = b;          \
        b = t;          \
} while (0)

#define CLI_GRAY "\e[90m"
#define CLI_REDD "\e[91m"
#define CLI_GREN "\e[92m"
#define CLI_YELW "\e[93m"
#define CLI_BLUE "\e[94m"
#define CLI_PRPL "\e[95m"
#define CLI_CYAN "\e[96m"
#define CLI_RSET "\e[m"

#define PP_NARG(...)  PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)

#define PP_ARG_N(                                         \
        _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,          \
        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
        _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
        _61, _62, _63, N, ...) N

#define PP_RSEQ_N()                         \
    63, 62, 61, 60,                         \
    59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define __JOIN_IMPL(a, b)                                                a ## b
#define PP_JOIN(a, b)                                                    __JOIN_IMPL(a, b)
#define __PP_APPLY1(macro, a)                                            macro(a)
#define __PP_APPLY2(macro, a, b)                                         macro(a) macro(b)
#define __PP_APPLY3(macro, a, b, c)                                      macro(a) macro(b) macro(c)
#define __PP_APPLY4(macro, a, b, c, d)                                   macro(a) macro(b) macro(c) macro(d)
#define __PP_APPLY5(macro, a, b, c, d, e)                                macro(a) macro(b) macro(c) macro(d) macro(e)
#define __PP_APPLY6(macro, a, b, c, d, e, f)                             macro(a) macro(b) macro(c) macro(d) macro(e) macro(f)
#define __PP_APPLY7(macro, a, b, c, d, e, f, g)                          macro(a) macro(b) macro(c) macro(d) macro(e) macro(f) macro(g)
#define __PP_APPLY8(macro, a, b, c, d, e, f, g, h)                       macro(a) macro(b) macro(c) macro(d) macro(e) macro(f) macro(g) macro(h)
#define __PP_APPLY9(macro, a, b, c, d, e, f, g, h, i)                    macro(a) macro(b) macro(c) macro(d) macro(e) macro(f) macro(g) macro(h) macro(i)
#define __PP_APPLY10(macro, a, b, c, d, e, f, g, h, i, j)                macro(a) macro(b) macro(c) macro(d) macro(e) macro(f) macro(g) macro(h) macro(i) macro(j)
#define __PP_APPLY11(macro, a, b, c, d, e, f, g, h, i, j, k)             macro(a) macro(b) macro(c) macro(d) macro(e) macro(f) macro(g) macro(h) macro(i) macro(j) macro(k)
#define __PP_APPLY12(macro, a, b, c, d, e, f, g, h, i, j, k, l)          macro(a) macro(b) macro(c) macro(d) macro(e) macro(f) macro(g) macro(h) macro(i) macro(j) macro(k) macro(l)
#define __PP_APPLY13(macro, a, b, c, d, e, f, g, h, i, j, k, l, m)       macro(a) macro(b) macro(c) macro(d) macro(e) macro(f) macro(g) macro(h) macro(i) macro(j) macro(k) macro(l) macro(m)
#define __PP_APPLY14(macro, a, b, c, d, e, f, g, h, i, j, k, l, m, n)    macro(a) macro(b) macro(c) macro(d) macro(e) macro(f) macro(g) macro(h) macro(i) macro(j) macro(k) macro(l) macro(m) macro(n)
#define __PP_APPLY15(macro, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) macro(a) macro(b) macro(c) macro(d) macro(e) macro(f) macro(g) macro(h) macro(i) macro(j) macro(k) macro(l) macro(m) macro(n) macro(o)
#define __PP_APPLY_(macro, M, ...)                                       M(macro, __VA_ARGS__)
#define PP_APPLY(macro, ...)                                             __PP_APPLY_(macro, PP_JOIN(__PP_APPLY, PP_NARG(__VA_ARGS__)), __VA_ARGS__)
#define VA_ARG_MANIP(macro, ...)                                         PP_APPLY(macro, __VA_ARGS__)

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#ifdef __clang__
#define nested(type, name, args) \
    type (^ name) args = NULL;   \
    name = ^ type args
#define nested_var(var)  typeof(var) var = (typeof(var)) 0;
#define nester_varm(var) typeof(var) var = {}

#else
#define nested(type, name, args) \
    type name args
#define nested_var(var)  (void)0
#define nester_varm(var) (void)0
#endif

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#define slladd(head, node) do {                 \
        var __n__ = &head;                      \
        while (*__n__) __n__ = &(*__n__)->next; \
        *__n__ = node;                          \
} while (0)

#define sllrem(head, node) do {                         \
        var __n__ = &head;                              \
        while (*__n__ != node) __n__ = &(*__n__)->next; \
        *__n__ = node->next;                            \
} while (0)

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#ifndef FOOL_THE_IDE

#define arrmove_r(arr, start, count) do {                     \
        var v = (arr)[(start) + (count) - 1];                 \
        for (int I = (count) + (start) - 1; I > (start); I--) \
        (arr)[I] = (arr)[I - 1];                              \
        (arr)[(start)] = v;                                   \
} while (0)

#define arrmove_l(arr, start, count) do {                     \
        var v = (arr)[(start)];                               \
        for (int I = (start); I < (count) + (start) - 1; I++) \
        (arr)[I] = (arr)[I + 1];                              \
        (arr)[(count) + (start) - 1] = v;                     \
} while (0)
#else

void arrmove_r(void* mem, int i, int n);
void arrmove_l(void* mem, int i, int n);
#endif

#endif