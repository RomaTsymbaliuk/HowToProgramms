#ifndef STACK_H
#define STACK_H
#include "stack.h"
#endif
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
#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif
#ifndef CONSTANTS
#define CONSTANTS
#include "constants.h"
#endif

int stack_init(struct data *d)
{
	printf("Stack initialization!\n");
	(d->anon_s).str_pnt = &Stack;
	struct stack *st = malloc(sizeof(struct stack));
	st->top = -1;
	d->anon_s.str_pnt = st;
//	printf("TOP : %d\n", ((struct stack*)d->anon_s.str_pnt)->top);
	if (d->data_type == STATIC_ARRAY) {		
		static_init(d);
//		printf("After init : %s\n", (char*)d->data_p[0]);
	} else if (d->data_type == DYNAMIC_ARRAY) {
		dynamic_init(d);
//		printf("After init : %s\n", (char*)d->data_p[0]);

	} else if (d->data_type == LINKED_LIST) {
		list_init(d);
//		printf("After init : %s\n", ((struct list*)d->anon_dt_tp.dat_type_pnt)->item);

	}
}
int stack_is_full(struct data *d)
{
	struct stack *st = d->anon_s.str_pnt;
	if (st -> top == d->structure_size) {
		return TRUE;
	}
	return FALSE;
}
int stack_push(struct data *d)
{
	if (stack_is_full(d) == FALSE) {
		struct stack *st = d->anon_s.str_pnt;
		d->data_p[++st->top] = d->act_data;
		printf("Pushed : %s\n", d->act_data);
		printf("Stack push!\n");
		return TRUE;
	}
	else {
		return FALSE;
	}
}
int stack_is_empty(struct data *d)
{
	struct stack *st = d->anon_s.str_pnt;
	if (st->top == -1) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}
int stack_pop(struct data *d)
{
	printf("Stack pop!\n");
	struct stack *st = d->anon_s.str_pnt;
	if (stack_is_empty(d) == FALSE) {
		d->data_p[st->top--] = NULL;

	}
}
int stack_upload(struct data *d)
{
	FILE *f = fopen("st.log", "a");
	struct stack *st = d->anon_s.str_pnt;
	if (f) {
		for (int i = 0; i <= st->top; i++) {
			fprintf(f, "%s\n", d->data_p[i]);
		}
		fclose(f);
	}
}
int stack_print(struct data *d)
{
	struct stack *st = d->anon_s.str_pnt;
	printf("Printing stack, top : %d \n", st->top);
	for (int i = 0; i <= st->top; i++) {
		printf("%s\n", d->data_p[i]);
	}
}
