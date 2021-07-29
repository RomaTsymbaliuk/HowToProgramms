#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif
#ifndef STACK_H
#define STACK_H
#include "stack.h"
#endif

//int push(struct data *d)
//{
	
//	
//}
//int pop(struct data *d)
//{
//
//}
int init(struct data *d)
{
	if (d->data_structure == STACK) {
		stack_init(d);
	}
}
//	else if (d->data_structure == QUEUE) {
//		queue_init(d);
//	}


//void print(struct data *d)
//{
//
//}
