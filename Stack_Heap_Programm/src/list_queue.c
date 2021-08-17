#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list_queue.h"

int list_queue_init(struct data *d)
{
	struct queue_list *q = (struct queue_list*)malloc(sizeof(struct queue_list));
	if (q) {
		q->item = NULL;
		q->next = NULL;
		d->data_type_pnt = q;
		return TRUE;
	} else {
		printf("Memory leak\n");
		return FALSE;
	}
	return FALSE;
}

int list_queue_push(struct data *d, void *data)
{
	struct queue_list *q = d->data_type_pnt;

	while (q->next != NULL) {

		q = q->next;
	}
	q->next = (struct queue_list*)malloc(sizeof(struct queue_list));
	if (q->next) {
		q = q->next;
		q->item = data;
		q->next = NULL;
		return TRUE;
	} else {
		printf("Memory leak\n");
	}
	return FALSE;
}

int list_queue_pop(struct data *d)
{
	struct queue_list *st = d->data_type_pnt;
	struct queue_list *q = st;	

	if (st->next) {
		q = st->next; 
		if (q->next) {
			st->next = q->next;
			q->next = NULL;
		} else {
			st->next = NULL;
		}
		if (q)
			free(q);
		
		return TRUE;
	} else {
		printf("List Queue empty!\n");
	}
	return FALSE;
}

int list_queue_print(struct data *d, int flag)
{
	struct queue_list *q = d->data_type_pnt;

	if (flag == TO_FILE) {

		if (d->filename_download) {

			FILE *f = fopen(d->filename_download, "w");
			if (!f)
				return FALSE;
			while(q->next != NULL) {

				q = q->next;
				if (q->item) 
					fprintf(f, "%s\n", (char*)q->item);
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
		while(q->next != NULL) {
			q = q->next;
			if (q->item) 
				printf("%s\n", (char*)q->item);
			else
				printf("\nNULL\n");
		}
		printf("======================================\n");
		return TRUE;
	}
	return FALSE;
	
}

int list_queue_upload(struct data *d)
{
	FILE *file;
	char line[256];

	if (d->filename_upload) {
		
		file = fopen(d->filename_upload, "r"); 

	   	if (file) {

		   while (fgets(line, sizeof(line), file)) {

		   		//delete last \n charachter
		   		line[strlen(line) - 1] = '\0'; 
		      	if (list_queue_push(d, strdup(line)) == FALSE) {
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

int list_queue_download(struct data *d)
{
	
	struct queue_list *q = d->data_type_pnt;
	if (q->next == NULL) {
		printf("No file creating for empty stack\n");
		return FALSE;
	}
	return list_queue_print(d, TO_FILE); 
}