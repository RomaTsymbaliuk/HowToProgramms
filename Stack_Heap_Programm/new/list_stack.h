#ifndef _LIST_STACK_H
#define _LIST_STACK_H

struct list{
	void *item;
	struct list *next;
}List;

struct stack_list {
	struct list *l;
}

list_stack_init(struct data *d);
list_stack_push(struct data *d);
list_stack_pop(struct data *d);
list_stack_is_empty(struct data *d);
list_stack_is_full(struct data *d);

#endif