#ifndef __LIST_STACK_H
#define __LIST_STACK_H
#include "type.h"
#include <stddef.h>

struct queue_list{
	void *item;
	struct queue_list *next;
};

int list_queue_init(struct data *d);
int list_queue_push(struct data *d, void *data);
int list_queue_pop(struct data *d);
int list_queue_print(struct data *d, int flag);
int list_queue_upload(struct data *d);
int list_queue_download(struct data *d);

static struct data l_stack_obj = {.data_type = LIST_STACK, 
								  .init = list_stack_init, .push = list_stack_push, 
								  .pop = list_stack_pop, .print = list_stack_print,
								  .upload = list_stack_upload, .download = list_stack_download,
								  .size = 0, .filename_upload = NULL, .filename_download = NULL};

#endif /* __LIST_STACK_H */