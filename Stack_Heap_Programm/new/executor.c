#ifndef EXECTR
#define EXECTR
#include "executor.h"
#endif
#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif

void executor(struct data *d)
{
	printf("Actual data in executor : %s\n", d->act_data);
	if (d->init) {
		d->init(d);
	} 
	if (d->push) {
		d->push(d);
	} 
	if (d->pop) {
		d->pop(d);
	} 
       	if (d->print) {
		d->print(d);
	}
}
