#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif
#ifndef STACK_H
#define STACK_H
#include "stack.h"
#endif
#ifndef QUEUE_H
#define QUEUE_H
#include "queue.h"
#endif

int init(struct data *d)
{
	if (d->data_structure == STACK) {
		stack_init(d);
	}
       	else {
		queue_init(d);
	}
}
int push(struct data *d)
{
	if (d->data_structure == STACK) {
		stack_push(d);
	}
	else {
		queue_push(d);
	}
}
int pop(struct data *d)
{
	if (d->data_structure == STACK) {
		stack_pop(d);
	}
	else {
		queue_pop(d);
	}
}
