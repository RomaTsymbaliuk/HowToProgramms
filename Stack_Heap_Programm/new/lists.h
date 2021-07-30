#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif
#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif

struct list{
	void *item;
	struct list *next;
}List;

void list_init_d(struct data *d);
