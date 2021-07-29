#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif
#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif
#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif
#ifndef STACK_H
#define STACK_H
#include "stack.h"
#endif
#ifndef TYPES_H
#define TYPES_H
#include "types.h"
#endif

int main()
{
	struct data *d = (struct data *)malloc(sizeof(struct data));
	d->data_type = STATIC_ARRAY;
	d->data_structure = STACK;
	init(d);
	return 1;
}
