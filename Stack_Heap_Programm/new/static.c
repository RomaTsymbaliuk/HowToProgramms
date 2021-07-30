#ifndef STATIC_H
#define STATIC_H
#include "static.h"
#endif

void static_init_d(struct data *d)
{
	printf("Static initialization!\n");
	printf("TOP : %d\n", ((struct stack*)d->anon_s.str_pnt)->top); 

}
