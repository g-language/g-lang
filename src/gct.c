#include "token.h"

int main(int n, char** arg) {
	token* tokens;
	memfile* mem = new(memfile);
	
	memfile_open(mem, "test/loadfile.gg", "r");
	tokens = tokenize(mem->data);
	
	while (tokens) {
		char bf[64] = { 0 };
		
		for (u32 i = 0; i < tokens->indent; i++)
			strcat(bf, "\t");
		
		if (tokens->firstword)
			warning("" PRNT_GRAY"line: " PRNT_REDD"%d", tokens->linenum);
		info("%s%s", bf, tokens->word);
		tokens = tokens->next;
	}
	
	warning("success");
}
