#include "dynamic_stack.h"
#include <stdlib.h>
#include <stdio.h>

int dynamic_stack_init(struct data *d)
{
	(d->anon_s).structure_pointer = &Dynamic_Stack;
	char **arr = (char**)malloc(sizeof(char*) * d->size);
	struct dynamic_stack *st = malloc(sizeof(struct dynamic_stack));
	st->top = -1;
	st->arr = arr;
	d->anon_s.structure_pointer = st;
	return TRUE;
}

int dynamic_stack_push(struct data *d)
{
	struct dynamic_stack *st = d->anon_s.structure_pointer;
	if (dynamic_stack_is_full(d) == FALSE) {
		st->arr[++st->top] = d->data_ptr;
		return TRUE;
	}
	return FALSE;
}
int dynamic_stack_is_empty(struct data *d)
{
	struct dynamic_stack *st = d->anon_s.structure_pointer;
	if (st->top == -1) {
		return TRUE;
	} else {
		return FALSE;
	}
}
int dynamic_stack_pop(struct data *d)
{
	struct dynamic_stack *st = d->anon_s.structure_pointer;
	if (dynamic_stack_is_empty(d) == FALSE) {
		st->arr[st->top--] = NULL;
		return TRUE;
	}
	return FALSE;
}
int dynamic_stack_is_full(struct data *d)
{
	struct dynamic_stack *st = d->anon_s.structure_pointer;
	if (st->top == d->size) {
		return TRUE;
	}
	return FALSE;
}
int dynamic_stack_print(struct data *d)
{
	struct dynamic_stack *st = d->anon_s.structure_pointer;
	for (int i = 0; i <= st->top; i++) {
		printf("%s\n", st->arr[i]);
	}
	return TRUE;
}
int dynamic_stack_upload(struct data *d)
{
	return TRUE;
}
int dynamic_stack_download(struct data *d)
{
	return TRUE;
}