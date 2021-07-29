#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif 
#ifndef TYPES_H
#define TYPES_H
#include "types.h"
#endif
#define CONST_SIZE 30
#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif

int static_init(struct data *d)
{
	printf("Static initialization!\n");
}
int dynamic_init(struct data *d)
{
	printf("Dynamic initialization!\n");
}
int list_init(struct data *d)
{
	printf("List initialization!\n");
}
