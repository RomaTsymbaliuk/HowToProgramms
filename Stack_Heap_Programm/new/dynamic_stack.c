#include "dynamic_stack.h"

void dynamic_init_d(struct data *d)
{
	char **arr = (char**)malloc(sizeof(char*) * d->structure_size);
}

int dynamic_stack_push(struct data *d)
{
	struct stack *st = d->anon_s.structure_pointer;
	if (dynamic_stack_is_full(d) == FALSE) {
		d->data_p[++st->top] = d->act_data;
		return TRUE;
	}
	return FALSE;
}
int dynamic_stack_is_empty(struct data *d)
{
	struct stack *st = d->anon_s.structure_pointer;
	if (st->top == -1) {
		return TRUE;
	} else {
		return FALSE;
	}
}
int dynamic_stack_pop(struct data *d)
{
	struct stack *st = d->anon_s.structure_pointer;
	if (dynamic_stack_is_empty(d) == FALSE) {
		d->data_p[st->top--] = NULL;
	}
}
int dynamic_stack_is_full(struct data *d)
{
	struct stack *st = d->anon_s.structure_pointer;
	if (st->top == d->structure_size) {
		return TRUE;
	}
}

