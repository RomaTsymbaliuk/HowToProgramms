#ifndef TYPES_H
#define TYPES_H
#include "types.h"
#endif

int static_init(struct data *d)
{
	static_init_d(d);
}
int dynamic_init(struct data *d)
{
	dynamic_init_d(d);
}
int list_init(struct data *d)
{
	list_init_d(d);
}
