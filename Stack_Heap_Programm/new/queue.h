#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif
#ifndef TYPES_H
#define TYPES_H
#include "types.h"
#endif
#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif
#ifndef QUEUE
#define QUEUE
#include "queue.h"
#endif

int queue_init(struct data *d);
int queue_push(struct data *d);
int queue_pop(struct data *d);
int queue_is_full(struct data *d);
int queue_is_empty(struct data *d);

