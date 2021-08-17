#ifndef __LIST_QUEUE_H
#define __LIST_QUEUE_H
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

static struct data l_queue_obj = {.data_type = LIST_QUEUE, 
								  .init = list_queue_init, .push = list_queue_push, 
								  .pop = list_queue_pop, .print = list_queue_print,
								  .upload = list_queue_upload, .download = list_queue_download,
								  .size = 0, .filename_upload = NULL, .filename_download = NULL};

#endif /* __LIST_QUEUE_H */