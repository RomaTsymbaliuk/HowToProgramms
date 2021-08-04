#include "list_stack.h"
#include <stdlib.h>
#include <stdio.h>

int list_stack_init(struct data *d)
{
	(d->anon_s).structure_pointer = &List_Stack;
	struct stack_list *st = (struct stack_list*)malloc(sizeof(struct stack_list));
	if (st) {
		st->item = NULL;
		st->next = NULL;
		d->anon_s.structure_pointer = st;
		return TRUE;
	}
	return FALSE;
}
int list_stack_push(struct data *d)
{
	struct stack_list *st = d->anon_s.structure_pointer;
	struct stack_list *tmp = st;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}
	if (tmp == st) {
		tmp->item = d->data_ptr;
	}
	else {
		tmp->next = (struct stack_list*)malloc(sizeof(struct stack_list));
		(tmp->next)->next = NULL;
		tmp->item = d->data_ptr;
	}
	return TRUE;
}
int list_stack_pop(struct data *d)
{
	struct stack_list *st = d->anon_s.structure_pointer;
	struct stack_list *q = st;	
	while (st->next != NULL ){
		q = st;
		st = st->next;
	}
	
	st = NULL;
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
	struct stack_list *st = d->anon_s.structure_pointer;
	while (st != NULL) {
		printf("%s\n", st->item);
		st = st->next;
	}
}