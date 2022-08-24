#include "mem.h"

struct post_free_node {
	struct post_free_node *next;
	void                  *data;
};

static struct post_free_node *sPostFreeNode;

void *pque(const void *data)
{
	struct post_free_node **node = &sPostFreeNode;
	struct post_free_node *new = zalloc(sizeof(struct post_free_node));
	
	while (*node)
		node = &(*node)->next;
	*node = new;
	
	return new->data = (void *)data;
}

void pfree(void)
{
	struct post_free_node **cur = &sPostFreeNode;
	struct post_free_node *kill;
	
	while (*cur)
	{
		kill = *cur;
		cur = &(*cur)->next;
		
		free(kill->data);
		free(kill);
	}
}