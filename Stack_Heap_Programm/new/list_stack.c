#include "list_stack.h"

int list_stack_init(struct data *d)
{
	struct stack_list *st = (struct stack_list*)malloc(sizeof(struct stack_list));
	struct list *l = (struct list *)malloc(sizeof(struct list));
	st->l = l;
}
int list_stack_push(struct data *d)
{
	
}