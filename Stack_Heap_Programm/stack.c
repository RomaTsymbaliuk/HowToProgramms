#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

enum {STATIC_ARRAY, DYNAMIC_ARRAY, LINKED_LIST};


int stack_init(struct stack *st, int flag_data_type, int size)
{
	char **arr = NULL;
//        if (size < 0)
//       		return 0;       
	if (flag_data_type == STATIC_ARRAY) {
		arr = (char**)malloc(sizeof(char*) * size);
		if (!arr)
			return 0;
		st->list = NULL;
	} else if (flag_data_type == DYNAMIC_ARRAY) {
		arr = (char**)malloc(sizeof(char*) * size);
		if (!arr){
			return 0;		
		}
		st->list = NULL;

	}
        else if (flag_data_type == LINKED_LIST) {
		printf("Im here or not ?!\n");	
		list_init(st);
		st->arr = NULL;
		st->top = -1;
		st->size = 0;
		printf("first arg : %s\n", (st->list)->item);
		return 1;
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

int stack_push(int flag_data_type, struct stack *st, char *str)
{
	if (flag_data_type == LINKED_LIST) {
		list_add(st, str);
		++st->top;
		return 1;
	}
	if (!stack_is_full(st)) {
		st->arr[++st->top] = str;
		return 1;
	}
	printf("No space in stack!\n");
	return 0;
}

int stack_print(int data_type, struct stack *st, int file_flag, char *filename)
{
	FILE *stream; 
	if (file_flag) {
		FILE *f = fopen(filename, "w");
		stream = f;
	} else {
		stream = stdout;
	}
	printf("\nPrinting stack\n");
	if (data_type == DYNAMIC_ARRAY || data_type == STATIC_ARRAY){
		for (int i = 0; i <= st->top; i++) {
			fprintf(stream, "%s\n", st->arr[i]);
		}
	}
	else {
		struct linked_list *tmp = st->list;
		while (tmp!= NULL) {
			printf("%s\n", tmp->item);
			tmp = tmp->next;
		}
	}
	return 1;
}

int stack_pop(int flag_data_type, struct stack *st)
{

	if (stack_is_empty(st)){
		if (flag_data_type == LINKED_LIST) {
			list_remove(st);
			st->top--;
			return 1;
		}
		else {
			st->arr[st->top--] = NULL;
		}
	}

}
