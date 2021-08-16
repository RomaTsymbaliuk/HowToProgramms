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
		static char *arr[MAX_STACK_SIZE] = {NULL};
		st->arr = arr;
		d->data_type_pnt = st;
		return TRUE;
	} else {
		printf("Memory leak\n");
		return FALSE;
	}	
	return FALSE; 
}

int static_stack_push(struct data *d, void *data)
{
	struct static_stack *st = (d->data_type_pnt);

	if (static_stack_is_full(d) == FALSE) {
		st->arr[++st->top] = data;

		return TRUE;
	}
	return FALSE;
}

int static_stack_is_empty(struct data *d)
{
	struct static_stack *st = (d->data_type_pnt);

	if (st->top == -1)
		return TRUE;
	
	return FALSE;
}

int static_stack_pop(struct data *d)
{
	struct static_stack *st = (d->data_type_pnt);

	if (static_stack_is_empty(d) == FALSE) {
		st->arr[st->top--] = NULL;
		return TRUE;
	}
	
	return FALSE;
}

int static_stack_is_full(struct data *d)
{
	struct static_stack *st = (d->data_type_pnt);

	if (st->top == MAX_STACK_SIZE) 
		return TRUE;
	
	return FALSE;	
}

int static_stack_print(struct data *d, int flag)
{
	struct static_stack *st = (d->data_type_pnt);

	if (flag == TO_FILE) {

		if (d->filename_download) { 

			FILE *f = fopen(d->filename_download, "w");
			if (!f) {
				printf("File opening error\n");
				return FALSE;
			}
			for (int i = 0; i <= st->top; i++) {
				
				if (st->arr[i]) 
					fprintf(f, "\n%s\n", (char*)st->arr[i]);
				else
					fprintf(f, "%s\n", "NULL");
			}

			fclose(f);

			return TRUE;
		} else {
			printf("No download file specified!\n");

			return FALSE;
		}

		return FALSE;

	} else if (flag == TO_STDOUT) {
		for (int i = 0; i <= st->top; i++) {
			if (st->arr[i]) 
				printf("\n%s\n", (char*)st->arr[i]);
			else
				printf("\nNULL\n");
		}
		printf("\n======================================\n");

		return TRUE;
	}
	
	return FALSE;
}

int static_stack_upload(struct data *d)
{
	FILE *file;
	char line[256];

	if (d->filename_upload) {
		
		file = fopen(d->filename_upload, "r");
	   	if (file) {

		   while (fgets(line, sizeof(line), file)) {
		   		//delete last \n symbol 
		   		line[strlen(line) - 1] = '\0';
		      	if (static_stack_push(d, strdup(line)) == FALSE) {
		      		return FALSE;
		      	} //check
		   }

		   fclose(file);
		   return TRUE;
		} 
	} else {
		printf("No upload file specified\n");
		return FALSE;
	}
	return FALSE;
}

int static_stack_download(struct data *d)
{
	return static_stack_print(d, TO_FILE);
}



