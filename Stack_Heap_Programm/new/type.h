#ifndef TYPE_H
#define TYPE_H
#define TRUE 1
#define FALSE 0
#define MAX_SIZE 100

#include "data.h"
#include <stddef.h>

enum {STATIC_ARR_STACK, DYNAMIC_ARR_STACK, LIST_STACK, STATIC_ARR_QUEUE, DYNAMIC_ARR_QUEUE, LIST_QUEUE};

struct obj {

	int data_type;
	void *data_ptr;
	int (*init)(struct data *d);
	int (*push)(struct data *d);
	int (*pop)(struct data *d);
	int (*print)(struct data *d);
};

#endif
