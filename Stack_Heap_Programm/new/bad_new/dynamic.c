#ifndef DYNAMIC_H
#define DYNAMIC_H
#include "dynamic.h"
#endif
#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif
#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif

void dynamic_init_d(struct data *d)
{
	char **arr = (char**)malloc(sizeof(char*) * d->structure_size);
	arr[0]="Check";
	d->data_p = arr;
	printf("Dynamic initialization\n");
}
