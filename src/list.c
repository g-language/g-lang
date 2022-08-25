#include "list.h"

void strlist_realloc(strlist *list, u32 num)
{
	list->num = num;
	list->item = realloc(list->item, sizeof(char *) * num);
}

void strlist_free(strlist *list)
{
	for (u32 i = 0; i < list->num; i++)
		free(list->item[i]);
	
	list->num = 0;
}

void strlist_add(strlist *list, const char *item)
{
	strlist_realloc(list, list->num + 1);
	list->item[list->num - 1] = strdup(item);
}

void strlist_insert(strlist *list, const char *item, u32 slot)
{
	strlist_add(list, item);
	if (slot >= list->num)
		return;
	
	arrmove_r(list->item, slot, list->num - slot);
}