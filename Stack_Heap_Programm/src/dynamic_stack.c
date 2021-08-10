#include "dynamic_stack.h"
#include <stdlib.h>
#include <stdio.h>

int dynamic_stack_init(struct data *d)
{
	char **arr = (char**)malloc(sizeof(char*) * d->size);
	if (arr) {
		struct dynamic_stack *st = malloc(sizeof(struct dynamic_stack));
		st->top = -1;
		st->arr = arr;
		d->data_type_pnt = st;
		return TRUE;
	}
	return FALSE;
}

int dynamic_stack_push(struct data *d, void *data)
{
	struct dynamic_stack *st = (struct dynamic_stack*)d->data_type_pnt;
	if (dynamic_stack_is_full(d) == FALSE) {
		st->arr[++st->top] = data;
		return TRUE;
	}
	return FALSE;
}
int dynamic_stack_is_empty(struct data *d)
{
	struct dynamic_stack *st = (struct dynamic_stack*)d->data_type_pnt;
	if (st->top == -1) {
		return TRUE;
	} else {
		return FALSE;
	}
}
int dynamic_stack_pop(struct data *d)
{
	struct dynamic_stack *st = (struct dynamic_stack*)d->data_type_pnt;
	if (dynamic_stack_is_empty(d) == FALSE) {
		st->arr[st->top--] = NULL;
		return TRUE;
	}
	return FALSE;
}
int dynamic_stack_is_full(struct data *d)
{
	struct dynamic_stack *st = (struct dynamic_stack*)d->data_type_pnt;
	if (st->top == d->size) {
		return TRUE;
	}
	return FALSE;
}
int dynamic_stack_print(struct data *d, int flag)
{
	struct dynamic_stack *st = (struct dynamic_stack*)d->data_type_pnt;
	
	if (flag == TO_FILE) {
		if (d->filename) {
			FILE *f = fopen(d->filename, "w");
			if (!f)
				return FALSE;
			for (int i = 0; i <= st->top; i++) {
				if (st->arr[i]) 
					fprintf(f, "\n%s\n", st->arr[i]);
				else
					fprintf(f, "\n%s\n", "NULL");
			}
			fclose(f);
		}
		
	} else if (flag == TO_STDOUT) {
		for (int i = 0; i <= st->top; i++) {
			if (st->arr[i]) 
				printf("\n%s\n", st->arr[i]);
			else
				printf("\nNULL\n");
		}
		printf("\n======================================\n");
	}
}
int dynamic_stack_upload(struct data *d)
{
	return TRUE;
}
int dynamic_stack_download(struct data *d)
{
	return static_stack_print(d, TO_FILE);
}