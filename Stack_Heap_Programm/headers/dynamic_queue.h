#ifndef __DYNAMIC_QUEUE_H
#define __DYNAMIC_QUEUE_H

#include "type.h"
#include <stddef.h>

struct dynamic_queue {
	int rear;
	int front;
	void **arr;
};

int dynamic_queue_init(struct data *d);
int dynamic_queue_push(struct data *d, void *data);
int dynamic_queue_pop(struct data *d);
int dynamic_queue_is_empty(struct data *d);
int dynamic_queue_is_full(struct data *d);
int dynamic_queue_print(struct data *d, int flag);
int dynamic_queue_upload(struct data *d);
int dynamic_queue_download(struct data *d);

static struct data d_queue_obj = {.data_type = STATIC_ARR_QUEUE, 
								  .init = dynamic_queue_init, .push = dynamic_queue_push, 
								  .pop = dynamic_queue_pop, .print = dynamic_queue_print,
								  .upload = dynamic_queue_upload, .download = dynamic_queue_download,
								  .size = 0, .filename_upload = NULL, .filename_download = NULL};

#endif /* __DYNAMIC_QUEUE_H */
