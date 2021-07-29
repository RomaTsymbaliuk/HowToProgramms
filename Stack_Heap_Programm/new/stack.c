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

int stack_init(struct data *d)
{
	printf("Stack initialization!\n");
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
