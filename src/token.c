#include "token.h"

//crustify
const char* gTokenPattern = {
	"("
			"\\.\\w{1,}"               "|"
			"\\#\\w{1,}"               "|"
			"\\w{1,}"                  "|"
			"\\*{1,}"                  "|"
			"\\\"[^\\\"]*\\\""         "|"
			"\\{"                      "|"
			"\\}"                      "|"
			"\\("                      "|"
			"\\)"                      "|"
			"<{1,}"                    "|"
			">{1,}"                    "|"
			"\\["                      "|"
			"\\]"                      "|"
			"\\,"                      "|"
			"\\;"                      "|"
			"\\:"                      "|"
			"[\\!\\?\\&\\+\\=\\-\\*\\/]{1,}"
	")"
};
//uncrustify

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
		
		if (*token->word == '(')
			token->type = TOKEN_OPEN_BRACE;
		else if (*token->word == ')')
			token->type = TOKEN_CLOSE_BRACE;
		else if (*token->word == '{')
			token->type = TOKEN_OPEN_CURLY_BRACE;
		else if (*token->word == '}')
			token->type = TOKEN_CLOSE_CURLY_BRACE;
		else if (chracpt(*token->word, "0123456789"))
			token->type = TOKEN_NUMBER;
		else if (regex(token->word, "(char|short|int|long|bool|void)", REGFLAG_START))
			token->type = TOKEN_TYPE;
		else if (regex(token->word, "(case|break|return|const|union|typedef|struct|enum|true|false|NULL)", REGFLAG_START))
			token->type = TOKEN_KEYWORD;
		else if (regex(token->word, "\\#\\w{1,}", REGFLAG_START))
			token->type = TOKEN_PREPROCESSOR;
		else if (token->word[0] == ',')
			token->type = TOKEN_COMMA;
		else if (chracpt(*token->word, "*+=!/&"))
			token->type = TOKEN_OPERATOR;
		
		node_add(tokenHead, token);
	}
	
	return tokenHead;
}
