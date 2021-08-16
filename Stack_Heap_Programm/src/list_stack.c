#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list_stack.h"

int list_stack_init(struct data *d)
{
	struct stack_list *st = (struct stack_list*)malloc(sizeof(struct stack_list));
	if (st) {
		st->item = NULL;
		st->next = NULL;
		d->data_type_pnt = st;
		return TRUE;
	} else {
		printf("Memory leak\n");
		return FALSE;
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
	} else {
		printf("Memory leak\n");
	}
	return FALSE;
}

int list_stack_pop(struct data *d)
{
	struct stack_list *st = d->data_type_pnt;
	struct stack_list *q = st;	

	if (st->next) {
		while (st->next != NULL ){
			q = st;
			st = st->next;
		}
	
		if (q)
			q->next = NULL;
		
		free(st); //check

		return TRUE;
	} else {
		printf("List empty!\n");
	}
	return FALSE;
	
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
			return TRUE;
		} else {
			printf("No download file specified!\n");
			return FALSE;
		}
	} else if (flag == TO_STDOUT) {
		while(st->next != NULL) {
			st = st->next;
			if (st->item) 
				printf("%s\n", (char*)st->item);
			else
				printf("\nNULL\n");
		}
		printf("\n======================================\n");
		return TRUE;
	}
	return FALSE;
	
}

int list_stack_upload(struct data *d)
{
	FILE *file;
	char line[256];

	if (d->filename_upload) {
		
		file = fopen(d->filename_upload, "r"); 

	   	if (file) {

		   while (fgets(line, sizeof(line), file)) {

		   		//delete last \n charachter
		   		line[strlen(line) - 1] = '\0'; 
		      	if (list_stack_push(d, strdup(line)) == TRUE) {
		      		return TRUE;
		      	} else {
		      		return FALSE;
		      	}
		}
		   fclose(file);
		   return TRUE;
		} else {
			printf("File opening error ! \n");
			return FALSE;
		}
	} else {
		printf("No upload file specified!\n");
		return FALSE;
	}

	return FALSE;
}

int list_stack_download(struct data *d)
{
	return list_stack_print(d, TO_FILE); 
}