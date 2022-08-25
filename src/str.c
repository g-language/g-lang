#include "str.h"
#include <regex.h>

void strlist_realloc(strlist* list, u32 num) {
	list->num = num;
	list->item = realloc(list->item, sizeof(char*) * num);
}

void strlist_free(strlist* list) {
	for (u32 i = 0; i < list->num; i++)
		free(list->item[i]);
	
	list->num = 0;
}

void strlist_add(strlist* list, const char* item) {
	strlist_realloc(list, list->num + 1);
	list->item[list->num - 1] = strdup(item);
}

void strlist_insert(strlist* list, const char* item, u32 slot) {
	strlist_add(list, item);
	if (slot >= list->num)
		return;
	
	arrmove_r(list->item, slot, list->num - slot);
}

char*regex(const char* str, const char* pattern, regexflag flag) {
	regex_t reg;
	regmatch_t* match;
	u32 matchNum = 0;
	char* ret = NULL;
	
	if (flag & REGFLAG_MATCH_NUM)
		matchNum = (flag & REGFLAG_NUMMASK );
	
	match = malloc(sizeof(void*) * (matchNum + 1));
	
	if (regcomp(&reg, pattern, REG_EXTENDED)) {
		warning("regex: compilation error");
		warning("pattern: \"%s\"", pattern);
		goto done;
	}
	if (regexec(&reg, str, matchNum + 1, match, 0))
		goto done;
	
	if (flag & REGFLAG_START)
		ret = (char*)str + match[matchNum].rm_so;
	
	else if (flag & REGFLAG_END)
		ret = (char*)str + match[matchNum].rm_eo;
	
	else if (flag & REGFLAG_COPY)
		ret = strndup(str + match[matchNum].rm_so, match[matchNum].rm_eo - match[matchNum].rm_so);
	
done:
	regfree(&reg);
	free(match);
	
	return ret;
}
