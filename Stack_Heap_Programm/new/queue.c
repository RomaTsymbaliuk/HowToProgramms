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

int queue_init(struct data *d)
{       
        printf("Queue initialization!\n");
         if (d->data_type == STATIC_ARRAY) {
                static_init(d);

        } else if (d->data_type == DYNAMIC_ARRAY) {
                dynamic_init(d);

        } else if (d->data_type == LINKED_LIST) {
                list_init(d);

        }

}       
int queue_push(struct data *d)
{
	printf("Queue push!\n");
}
int queue_pop(struct data *d)
{
	printf("Queue pop!\n");
}
int queue_is_full(struct data *d)
{
	printf("Queue if full!\n");
}
int queue_is_empty(struct data *d)
{
	printf("Queue us empty!\n");
}

            
