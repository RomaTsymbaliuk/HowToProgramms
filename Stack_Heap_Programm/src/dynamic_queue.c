#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dynamic_queue.h"

int dynamic_queue_init(struct data *d)
{
	struct dynamic_queue *q = malloc(sizeof(struct dynamic_queue));
	char **arr = (char**)malloc(sizeof(char*) * d->size);
	
	if (!arr) {
		printf("Memory leak!\n");
		return FALSE;
	}
	
	if (q) {
		q->rear = -1;
		q->front = -1;
		q->arr = (void **)arr;
		d->data_type_pnt = (void *)q;
		return TRUE;
	} else {
		printf("Memory leak");
		return FALSE;
	}
	
	return FALSE;
}

int dynamic_queue_is_full(struct data *d)
{
	struct dynamic_queue *q = d->data_type_pnt;
	
	if (q->rear == d->size - 1) {
		printf("Queue is full!\n");
		return TRUE;
	}
	
	return FALSE;
}

int dynamic_queue_is_empty(struct data *d)
{
	struct dynamic_queue *q = d->data_type_pnt;
	
	if (q->front == -1 || q->front > q->rear) {
		printf("Queue is empty!\n");
		return TRUE;
	}
	
	return FALSE;
}

int dynamic_queue_push(struct data *d, void *data)
{
	struct dynamic_queue *q = d->data_type_pnt;

	if (q->front == -1) {
		q->front = 0;
	}
	if (dynamic_queue_is_full(d) == FALSE) {
		q->arr[++q->rear] = data;
		
		return TRUE;
	}
	
	return FALSE;
}

int dynamic_queue_pop(struct data *d)
{
	struct dynamic_queue *q = d->data_type_pnt;
	if (dynamic_queue_is_empty(d) == FALSE) {
		q->front++;
		return TRUE;
	}
	return FALSE;
}

int dynamic_queue_print(struct data *d, int flag)
{
	struct dynamic_queue *q = (d->data_type_pnt);
	FILE *f;
	
	if (flag == TO_FILE) {

		if (d->filename_download) { 

			f = fopen(d->filename_download, "w");
			if (!f) {
				printf("File opening error\n");
				return FALSE;
			}
			for (int i = q->front; i <= q->rear; i++) {
				
				if (q->arr[i]) 
					fprintf(f, "%s\n", (char*)q->arr[i]);
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
		for (int i = q->front; i <= q->rear; i++) {
			if (q->arr[i]) 
				printf("%s\n", (char*)q->arr[i]);
			else
				printf("NULL\n");
		}
		printf("\n======================================\n");

		return TRUE;
	}
	
	return FALSE;
}

int dynamic_queue_upload(struct data *d)
{
	FILE *file;
	char line[256];

	if (d->filename_upload) {
		
		file = fopen(d->filename_upload, "r");
	   	if (file) {

		   while (fgets(line, sizeof(line), file)) {
		   		//delete last \n symbol 
		   		line[strlen(line) - 1] = '\0';
		      	if (dynamic_queue_push(d, strdup(line)) == FALSE) {
		      		return FALSE;
		      	} 
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

int dynamic_queue_download(struct data *d)
{
	if (dynamic_queue_is_empty(d) == TRUE) {
		printf("Not creating files for an empty queue\n");
		return FALSE;
	}
	return dynamic_queue_print(d, TO_FILE);
}
