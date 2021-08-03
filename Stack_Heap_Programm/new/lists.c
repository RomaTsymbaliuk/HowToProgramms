#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif
#ifndef LISTS_H
#define LISTS_H
#include "lists.h"
#endif

void list_stack_init(struct data *d)
{
	struct list *l = (struct list*)malloc(sizeof(struct list));
	if (!l) {
		printf("Allocation list problem\n");
	}
	l->next = NULL;
	char *init_item = "List item";
	l->item = init_item;
	(d->anon_dt_tp).dat_type_pnt = &List;
        d->anon_dt_tp.dat_type_pnt = l;	

	printf("List initialization!\n");
}
