#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif
#ifndef SUCCESS
#define SUCCESS 0
#endif
#ifndef FAIL
#define FAIL 1
#endif

struct stack{
        int top;
        int size;
	struct data *d;
} Stack;

int stack_init(struct data *d);
int stack_push(struct data *d);
int stack_pop(struct data *d);
int stack_is_full(struct data *d);
int stack_is_empty(struct data *d);

	

