#include "static_stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

int static_stack_init(struct data *d)
{

	struct static_stack *st = malloc(sizeof(struct static_stack));
	if (st) {
		st->top = -1;
		static char *arr[MAX_SIZE] = {NULL};
		st->arr = arr;
		d->data_type_pnt = st;
		return TRUE;
	}
	

}
int static_stack_push(struct data *d, void *data)
{
	
	struct static_stack *st = (struct static_stack*)(d->data_type_pnt);
	if (static_stack_is_full(d) == FALSE) {
		st->arr[++st->top] = data;

		return TRUE;
	}
	return FALSE;
}
int static_stack_is_empty(struct data *d)
{
	
	struct static_stack *st = (struct static_stack*)(d->data_type_pnt);
	if (st->top == -1)
		return TRUE;
	else 
		return FALSE;
	
}
int static_stack_pop(struct data *d)
{

	struct static_stack *st = (struct static_stack*)(d->data_type_pnt);
	if (static_stack_is_empty(d) == FALSE) {
		st->arr[st->top--] = NULL;
	}
	
	return TRUE;
	
}
int static_stack_is_full(struct data *d)
{
	struct static_stack *st = (struct static_stack*)(d->data_type_pnt);

		if (st->top == MAX_SIZE) 
			return TRUE;
	
	return FALSE;
	
}
int static_stack_print(struct data *d, int flag)
{
	struct static_stack *st = (struct static_stack*)(d->data_type_pnt);

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
	}
	
	return TRUE;
}

int static_stack_upload(struct data *d)
{
	return TRUE;
}

int static_stack_download(struct data *d)
{
	return static_stack_print(d, TO_FILE);
}
