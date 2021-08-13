#include "list_stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	if (st->next) {

		st = st->next;
		st->item = data;
		st->next = NULL;
		return TRUE;
	}
	return FALSE;
}

int list_stack_pop(struct data *d)
{
	struct stack_list *st = d->data_type_pnt;
	struct stack_list *q = st;	

	while (st->next != NULL ){
		q = st;
		st = st->next;
	}
	
	if (q)
		q->next = NULL;
	
	free(st);
	
}

int list_stack_upload(struct data *d)
{
	if (d->filename_upload) {
		
		FILE *file = fopen(d->filename_upload, "r");
	   	char line[256];

	   	if (file) {

		   while (fgets(line, sizeof(line), file)) {

		   		line[strlen(line) - 1] = '\0';
		      	list_stack_push(d, strdup(line));
		}
		   fclose(file);
		   return TRUE;
		} else {

			printf("File opening error ! \n");
			return FALSE;
		}
	}
}

int list_stack_download(struct data *d)
{
	return list_stack_print(d, TO_FILE);
}

int list_stack_print(struct data *d, int flag)
{
	struct stack_list *st = d->data_type_pnt;

	if (flag == TO_FILE) {

		if (d->filename_download) {

			FILE *f = fopen(d->filename_download, "w");
			if (!f)
				return FALSE;
			while(st->next != NULL) {

				st = st->next;
				if (st->item) 
					fprintf(f, "%s\n", (char*)st->item);
				else
					fprintf(f, "%s\n", "NULL");
			}
		fclose(f);
		}
	} else if (flag == TO_STDOUT) {

		while(st->next != NULL) {

			st = st->next;
			if (st->item) 
				printf("%s\n", (char*)st->item);
			else
				printf("\nNULL\n");
		}
	}
	printf("\n======================================\n");
}