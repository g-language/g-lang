#ifndef TOKEN_H
#define TOKEN_H

void token_init(const char* input, const size_t sz);
char* token_next(void);

#endif