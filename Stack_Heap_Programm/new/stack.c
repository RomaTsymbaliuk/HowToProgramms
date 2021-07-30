#ifndef STACK_H
#define STACK_H
#include "stack.h"
#endif
#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif
#ifndef TYPES_H
#define TYPES_H
#include "types.h"
#endif
#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif
#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif

int stack_init(struct data *d)
{
	printf("Stack initialization!\n");
	(d->anon_s).str_pnt = &Stack;
	struct stack *st = malloc(sizeof(struct stack));
	st->top = -1;
	d->anon_s.str_pnt = st;
	printf("TOP : %d\n", ((struct stack*)d->anon_s.str_pnt)->top);
	if (d->data_type == STATIC_ARRAY) {		
		static_init(d);

	} else if (d->data_type == DYNAMIC_ARRAY) {
		dynamic_init(d);

	} else if (d->data_type == LINKED_LIST) {
		list_init(d);

	}
}
int stack_push(struct data *d)
{
	printf("Stack push!\n");
}
int stack_pop(struct data *d)
{
	printf("Stack pop!\n");
}
int stack_is_full(struct data *d)
{
	printf("Stack is full!\n");
}
int stack_is_empty(struct data *d)
{
	printf("Stack is empty!\n");
}
