#include "token.h"

//crustify
const char* sColList[] = {
	[TOKEN_VARIABLE]          = PRNT_RSET,
	[TOKEN_FUNCTION]          = PRNT_REDD,
	[TOKEN_COMMA]             = PRNT_RSET,
	[TOKEN_OPEN_BRACE]        = PRNT_RSET,
	[TOKEN_CLOSE_BRACE]       = PRNT_RSET,
	[TOKEN_OPEN_CURLY_BRACE]  = PRNT_RSET,
	[TOKEN_CLOSE_CURLY_BRACE] = PRNT_RSET,
	[TOKEN_TYPE]              = PRNT_CYAN,
	[TOKEN_KEYWORD]           = PRNT_YELW,
	[TOKEN_PREPROCESSOR]      = PRNT_PRPL,
	[TOKEN_OPERATOR]          = PRNT_GREN,
	[TOKEN_NUMBER]            = PRNT_GREN,
};
//uncrustify

int main(int n, char** arg) {
	token* tokens;
	memfile* mem = new(memfile);
	
	memfile_open(mem, "test/loadfile.gg", "r");
	tokens = tokenize(mem->data);
	
	while (tokens) {
		if (tokens->firstword) {
			char bf[64] = { 0 };
			
			for (u32 i = 0; i < tokens->indent; i++)
				strcat(bf, "\t");
			printf("%s", bf);
		}
		
		printf("" PRNT_GRAY "%s%s ", sColList[tokens->type], tokens->word);
		tokens = tokens->next;
		
		if (tokens && tokens->firstword)
			printf("\n");
	}
	printf("\n");
	
	warning("success");
}
