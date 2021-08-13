#ifndef __DYNAMIC_STACK_H
#define __DYNAMIC_STACK_H
#include "type.h"
#include <stddef.h>

struct dynamic_stack {
	int top;
	int size;
	void **arr;
}Dynamic_Stack;

int dynamic_stack_push(struct data *d, void *data);
int dynamic_stack_pop(struct data *d);
int dynamic_stack_is_full(struct data *d);
int dynamic_stack_is_empty(struct data *d);
int dynamic_stack_download(struct data *d);
int dynamic_stack_upload(struct data *d);
int dynamic_stack_print(struct data *d, int flag);
int dynamic_stack_init(struct data *d);

static struct data d_stack_obj = {.data_type = DYNAMIC_ARR_STACK, 
								  .init = dynamic_stack_init, .push = dynamic_stack_push, 
								  .pop = dynamic_stack_pop, .print = dynamic_stack_print,
								  .upload = dynamic_stack_upload, .download = dynamic_stack_download, 
								  .size = 0, .filename_upload = NULL, .filename_download = NULL};
#endif /* __DYNAMIC_STACK_H  */