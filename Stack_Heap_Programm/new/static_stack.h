#ifndef _STATIC_STACK_H
#define _STATIC_STACK_H

#include "type.h"

struct static_stack {
	int top;
	void **arr;
}Static_Stack;

int static_stack_init(struct data *d);
int static_stack_push(struct data *d);
int static_stack_pop(struct data *d);
int static_stack_is_empty(struct data *d);
int static_stack_is_full(struct data *d);

static struct obj s_stack_obj = {.data_ptr = NULL, .data_type = STATIC_ARR_STACK, .init = static_stack_init, .push = static_stack_push, .pop = static_stack_pop};

#endif /* _STATIC_STACK_H */

