#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

enum {STATIC_ARRAY, DYNAMIC_ARRAY, LINKED_LIST};


int stack_init(struct stack *st, int flag_data_type, int size)
{
	char **arr = NULL;
        if (size < 0)
       		return 0;       
	if (flag_data_type == STATIC_ARRAY) {
		arr = (char**)malloc(sizeof(char*) * size);
		if (!arr)
			return 0;
	} else if (flag_data_type == DYNAMIC_ARRAY) {
		arr = (char**)malloc(sizeof(char*) * size);
		if (!arr){
			return 0;		
		}

	}
        else {
		return 0;
	}	
	st->arr = arr;
	st->top = -1;
	st->size = size;	
	return 1;
}

/*
 * fun: sdfsdfsdf
 * @param1 - asdasdasds asd
 *
 * return 1 - ok, 0 - err
 */
int stack_is_full(struct stack *st)
{
	if (!st)
		return 0;
	if (st->top == st->size) {
		printf("stack is full !\n");
		return 1;
	} else {
		return 0;
	}
	return 1;
}
int stack_is_empty(struct stack *st)
{
	if (st->top == -1){
		printf("stack is empty !\n");
		return 0;
	}
	return 1;
}

int stack_push(struct stack *st, char *str)
{
	if (!stack_is_full(st)) {
		st->arr[++st->top] = str;
		return 1;
	}
	printf("No space in stack!\n");
	return 0;
}

int stack_print(struct stack *st, int file_flag, char *filename)
{
	FILE *stream;
	if (file_flag) {
		FILE *f = fopen(filename, "w");
		stream = f;
	} else {
		stream = stdout;
	}
	printf("\nPrinting stack\n");
	for (int i = 0; i <= st->top; i++) {
		fprintf(stream, "%s\n", st->arr[i]);
	}
	return 1;
}

int stack_pop(struct stack *st)
{
	if (stack_is_empty(st)) {
		st->arr[st->top--] = NULL;
		return 1;
	}
	printf("\nStack is empty!\n");
	return 0;

}
