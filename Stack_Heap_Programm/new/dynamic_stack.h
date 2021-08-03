#ifndef _DYNAMIC_STACK_H
#define _DYNAMIC_STACK_H

int dynamic_stack_push(struct data *d);
int dynamic_stack_pop(struct data *d);
int dynamic_stack_is_full(struct data *d);
int dynamic_stack_is_empty(struct data *d);

#endif