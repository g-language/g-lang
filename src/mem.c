#include "mem.h"

struct post_free_node {
	struct post_free_node *next;
	void                  *data;
};

static struct post_free_node *sPostFreeNode;

void *que(const void *data)
{
	struct post_free_node *new = zalloc(sizeof(struct post_free_node));
	nodeadd(sPostFreeNode, new);
	return new->data = (void *)data;
}

void quefree(void)
{
	while (sPostFreeNode)
	{
		free(sPostFreeNode->data);
		nodefree(sPostFreeNode, sPostFreeNode);
	}
}