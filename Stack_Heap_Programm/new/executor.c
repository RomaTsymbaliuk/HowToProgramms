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
	if (d->init) {
		d->init(d);
	} else if (d->push) {
		d->push(d);
	} else if (d->pop) {
		d->pop(d);
	} else if (d->print) {
		d->print(d);
	}
}
