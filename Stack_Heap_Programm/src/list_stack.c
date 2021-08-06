#include "list_stack.h"
#include <stdlib.h>
#include <stdio.h>

int list_stack_init(struct data *d)
{
	struct stack_list *st = (struct stack_list*)malloc(sizeof(struct stack_list));
	if (st) {
		st->item = NULL;
		st->next = NULL;
		d->data_type_pnt = st;
		return TRUE;
	}
	return FALSE;
}
int list_stack_push(struct data *d, void *data)
{
	struct stack_list *st = d->data_type_pnt;
	while (st->next != NULL) {
		st = st->next;
	}
	st->next = (struct stack_list*)malloc(sizeof(struct stack_list));
	st = st->next;
	st->item = data;
	st->next = NULL;
	return TRUE;
}
int list_stack_pop(struct data *d)
{
	struct stack_list *st = d->data_type_pnt;
	struct stack_list *q = st;	
	while (st->next != NULL ){
		q = st;
		st = st->next;
	}
	
	q->next = NULL;
	free(st);
	
}
int list_stack_upload(struct data *d)
{
	return TRUE;
}
int list_stack_download(struct data *d)
{
	return TRUE;
}
int list_stack_print(struct data *d)
{
	struct stack_list *st = d->data_type_pnt;
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
	while (st->next != NULL) {
		st = st->next;
		fprintf(stream, "%s\n", st->item);
	}
	if (stream)
		fclose(stream);
}