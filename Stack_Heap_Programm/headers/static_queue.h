#ifndef __STATIC_QUEUE_H
#define __STATIC_QUEUE_H

#include <stddef.h>
#include "type.h"

struct static_queue {
	int rear;
	int front;
	void **arr;
};

int static_queue_init(struct data *d);
int static_queue_push(struct data *d, void *data);
int static_queue_pop(struct data *d);
int static_queue_is_empty(struct data *d);
int static_queue_is_full(struct data *d);
int static_queue_print(struct data *d, int flag);
int static_queue_upload(struct data *d);
int static_queue_download(struct data *d);

static struct data s_queue_obj = {.data_type = STATIC_ARR_QUEUE,
								  .init = static_queue_init, .push = static_queue_push,
								  .pop = static_queue_pop, .print = static_queue_print,
								  .upload = static_queue_upload, .download = static_queue_download,
								  .size = MAX_QUEUE_SIZE, .filename_upload = NULL, .filename_download = NULL};

#endif /* __STATIC_QUEUE_H */
