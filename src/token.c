#include "token.h"

// const char *gTokenPattern = "(\\w{1,}|\\*{1,}|\\{|\\}|\\(|\\)|\\[|\\]|\\,|\\;|\\:|\\\"[^\\\"]*\\\")";
const char* gTokenPattern = {
	"("
	"\\.\\w{1,}"
	"|"
	"\\#\\w{1,}"
	"|"
	"\\w{1,}"
	"|"
	"\\*{1,}"
	"|"
	"\\{"
	"|"
	"\\}"
	"|"
	"\\("
	"|"
	"\\)"
	"|"
	"\\["
	"|"
	"\\]"
	"|"
	"\\,"
	"|"
	"\\;"
	"|"
	"\\:"
	"|"
	"\\\"[^\\\"]*\\\""
	")"
	
};

static inline char*next_token(const char* str) {
	return regex(str, gTokenPattern, REGFLAG_START);
}

static inline char*copy_token(const char* str) {
	return regex(str, gTokenPattern, REGFLAG_COPY);
}

token* tokenize(const char* str) {
	token* tokenHead = NULL;
	token* token;
	const char* head = str;
	const char* ind;
	u32 addoff = 0;
	
	while ((str = next_token(str + addoff))) {
		token = zalloc(sizeof(struct token));
		ind = linehead(head, str);
		
		while (*ind == '\t') {
			token->indent++;
			ind++;
		}
		
		if (ind == str)
			token->firstword = true;
		
		token->linenum = linenum(head, str);
		token->charnum = charnum(head, str);
		token->word = copy_token(str);
		addoff = strlen(token->word);
		
		node_add(tokenHead, token);
	}
	
	return tokenHead;
}
