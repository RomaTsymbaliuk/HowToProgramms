#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

enum {STATIC_ARRAY, DYNAMIC_ARRAY, LINKED_LIST};

int queue_is_full(struct queue *q)
{
	if (q->head == q->size - 1) {
		return 0;
	}
	return 1;
}
int queue_is_empty(struct queue *q)
{
	if (q->tail == -1 || q->tail > q->head) {
		return 0;
	}
	return 1;
}
int queue_push(struct queue *q, char *str) {
	if (queue_is_full(q)) {
		if (q->tail == -1) {
			q->tail = 0;
		}
		q->arr[q->head++] = str;
		return 1;
	}
}
int queue_pop(struct queue *q) {
	if (queue_is_empty(q)) {
		q->tail++;
	}
}
void queue_show(struct queue *q) {
	if (!queue_is_empty(q)) {
		printf("queue is empty");
	} else {
		for (int i = q->tail; i <= q->head; i++) {
			printf("%s ", q->arr[i]);
		}
		printf("\n");
	}
}
int queue_init(struct queue *q, int flag_data_type, int size) {

        char **arr = NULL;
//        if (size < 0)
//                      return 0;       
        if (flag_data_type == STATIC_ARRAY) {
                arr = (char**)malloc(sizeof(char*) * size);
                if (!arr)
                        return 0;
        } else if (flag_data_type == DYNAMIC_ARRAY) {
                arr = (char**)malloc(sizeof(char*) * size);
                if (!arr){
                        return 0;
                }

        } else if (flag_data_type == LINKED_LIST) {
                list_init(NULL, q);
                q->size = 0;
		q->head = -1;
		q->tail = -1;
                return 1;
	}	
//	q->head = -1;
//      q->tail = -1;
//	q->arr = arr;
//	q->list = NULL;
      q->size = size;
        return 1;

}
