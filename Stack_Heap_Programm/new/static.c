#ifndef STATIC_H
#define STATIC_H
#include "static.h"
#endif
#define MAX_SIZE 100

void static_init_d(struct data *d)
{
	printf("Static initialization!\n");
	static char *arr[MAX_SIZE]={""};
	d->data_p = (char**)arr;
}
