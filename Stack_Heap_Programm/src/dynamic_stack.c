#include "dynamic_stack.h"
#include <stdlib.h>
#include <stdio.h>

int dynamic_stack_init(struct data *d)
{
	char **arr = (char**)malloc(sizeof(char*) * d->size);
	struct dynamic_stack *st = malloc(sizeof(struct dynamic_stack));
	st->top = -1;
	st->arr = arr;
	d->data_type_pnt = st;
	return TRUE;
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
	struct dynamic_stack *st = (struct dynamic_stack*)d->data_type_pnt;;
	if (st->top == d->size) {
		return TRUE;
	}
	return FALSE;
}
int dynamic_stack_print(struct data *d)
{
	struct dynamic_stack *st = (struct dynamic_stack*)d->data_type_pnt;
	FILE *stream;
	if (d->filename) {
		FILE *f = fopen(d->filename, "w");
		if (!f)
			return FALSE;
		stream = f;
	}
	else {
		stream = stdout;
	}
	for (int i = 0; i <= st->top; i++) {
		fprintf(stream, "%s\n", st->arr[i]);
	}
	if (stream)
		fclose(stream);
	return TRUE;
}
int dynamic_stack_upload(struct data *d)
{
	return TRUE;
}
int dynamic_stack_download(struct data *d)
{
	return TRUE;
}