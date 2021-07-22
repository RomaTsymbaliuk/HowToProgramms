#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

enum {STATIC_ARRAY, DYNAMIC_ARRAY, LINKED_LIST};
int stack_init(struct stack *st, int flag_data_type, int size)
{ 
	if (flag_data_type == STATIC_ARRAY) {
		char *arr[size];
		st->arr = arr;
	} else if (flag_data_type == DYNAMIC_ARRAY) {
		char **arr = (char**)malloc(sizeof(char*) * size);
		st->arr = arr;
	} 
	st->top = -1;
	st->size = size;
}
int stack_is_full(struct stack *st)
{
	if (st->top == st->size) {
		printf("stack is full !\n");
		return 1;
	} else {
		return 0;
	}
}
int stack_is_empty(struct stack *st)
{
	if (st->top == -1){
		printf("stack is empty !\n");
		return 1;
	} else {
		return 0;
	}
}
