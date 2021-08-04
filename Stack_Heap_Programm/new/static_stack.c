#include "static_stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

int static_stack_init(struct data *d)
{
	
	(d->anon_s).structure_pointer = &Static_Stack;
	struct static_stack *st = malloc(sizeof(struct static_stack));
	st->top = -1;
	static char *arr[MAX_SIZE] = {""};
	st->arr = arr;
	d->anon_s.structure_pointer = st;

	return TRUE;
}
int static_stack_push(struct data *d)
{
	
	struct static_stack *st = d->anon_s.structure_pointer;
	if (static_stack_is_full(d) == FALSE) {
		st->arr[++st->top] = d->data_ptr;

		return TRUE;
	}

	return FALSE;
}
int static_stack_is_empty(struct data *d)
{
	
	struct static_stack *st = d->anon_s.structure_pointer;
	if (st->top == -1) {

		return TRUE;
	} else {

		return FALSE;
	}
	
}
int static_stack_pop(struct data *d)
{
	
	struct static_stack *st = d->anon_s.structure_pointer;
	
	if (static_stack_is_empty(d) == FALSE) {
		st->arr[st->top--] = NULL;
	}
	
	return TRUE;
	
}
int static_stack_is_full(struct data *d)
{
	
	struct static_stack *st = d->anon_s.structure_pointer;
		if (st->top == d->size) {
		return TRUE;
	}
	return FALSE;
	
}
int static_stack_print(struct data *d)
{
	struct static_stack *st = d->anon_s.structure_pointer;
	for (int i = 0; i <= st->top; i++) {
		printf("\n%s\n", st->arr[i]);
	}
}
