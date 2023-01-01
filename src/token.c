#include "lbolib.h"
#define STB_C_LEXER_IMPLEMENTATION
#include "stb/lexer.h"

#include "token.h"

typedef stb_lexer lexer_t;

struct tokenctx_t {
    lexer_t lex;
    
    char*  tmp;
    size_t stmp;
};

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

struct tokenctx_t this;

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

void token_init(const char* input, const size_t sz) {
    const size_t bs = 1024 * 8;
    
    this.tmp = new(char[bs]);
    stb_c_lexer_init(&this.lex, input, input + sz, this.tmp, bs);
}

static char* token_print(lexer_t* lexer) {
    char* s = NULL;
    
    switch (lexer->token) {
        case CLEX_id: s = x_fmt("%s", lexer->string); break;
        case CLEX_eq: s = x_strdup("=="); break;
        case CLEX_noteq: s = x_strdup("!="); break;
        case CLEX_lesseq: s = x_strdup("<="); break;
        case CLEX_greatereq: s = x_strdup(">="); break;
        case CLEX_andand: s = x_strdup("&&"); break;
        case CLEX_oror: s = x_strdup("||"); break;
        case CLEX_shl: s = x_strdup("<<"); break;
        case CLEX_shr: s = x_strdup(">>"); break;
        case CLEX_plusplus: s = x_strdup("++"); break;
        case CLEX_minusminus: s = x_strdup("--"); break;
        case CLEX_arrow: s = x_strdup("->"); break;
        case CLEX_andeq: s = x_strdup("&="); break;
        case CLEX_oreq: s = x_strdup("|="); break;
        case CLEX_xoreq: s = x_strdup("^="); break;
        case CLEX_pluseq: s = x_strdup("+="); break;
        case CLEX_minuseq: s = x_strdup("-="); break;
        case CLEX_muleq: s = x_strdup("*="); break;
        case CLEX_diveq: s = x_strdup("/="); break;
        case CLEX_modeq: s = x_strdup("%%="); break;
        case CLEX_shleq: s = x_strdup("<<="); break;
        case CLEX_shreq: s = x_strdup(">>="); break;
        case CLEX_eqarrow: s = x_strdup("=>"); break;
        case CLEX_dqstring: s = x_fmt("\"%s\"", lexer->string); break;
        case CLEX_sqstring: s = x_fmt("'\"%s\"'", lexer->string); break;
        case CLEX_charlit: s = x_fmt("'%s'", lexer->string); break;
#if defined(STB__clex_int_as_double) && !defined(STB__CLEX_use_stdlib)
        case CLEX_intlit: s = x_fmt("%g", lexer->real_number); break;
#else
        case CLEX_intlit: s = x_fmt("%ld", lexer->int_number); break;
#endif
        case CLEX_floatlit: s = x_fmt("%g", lexer->real_number); break;
        default:
            if (lexer->token >= 0 && lexer->token < 256)
                s = x_fmt("%c", (int) lexer->token);
            else {
                s = x_fmt("<<<UNKNOWN TOKEN %ld >>>\n", lexer->token);
            }
            break;
    }
    
    return s;
}

char* token_next(void) {
    lexer_t* lex = &this.lex;
    int tkn = stb_c_lexer_get_token(&this.lex);
    
    if (!tkn || lex->token == CLEX_parse_error) return NULL;
    
    return token_print(lex);
}
