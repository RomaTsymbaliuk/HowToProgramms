#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "static_queue.h"

int static_queue_init(struct data *d)
{
	struct static_queue *q = malloc(sizeof(struct static_queue));
	static char *arr[MAX_QUEUE_SIZE] = {NULL};

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

int static_queue_is_full(struct data *d)
{
	struct static_queue *q = d->data_type_pnt;
	int full_flag = TRUE;

	for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
		if (!q->arr[i])
			full_flag = FALSE;
	}

	return full_flag;
}

int static_queue_is_empty(struct data *d)
{
	struct static_queue *q = d->data_type_pnt;

	int empty_flag = TRUE;
	for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
		if (q->arr[i])
			empty_flag = FALSE;
	}

	return empty_flag;
}

int static_queue_push(struct data *d, void *data)
{
	struct static_queue *q = d->data_type_pnt;

	if (q->front == -1) {
		q->front = 0;
	}
	if (static_queue_is_full(d) == FALSE) {
		
		if (!q->arr[(++q->rear) % MAX_QUEUE_SIZE]) {
			q->arr[(q->rear) % MAX_QUEUE_SIZE] = data;
		}

		return TRUE;
	}

	return FALSE;
}

int static_queue_pop(struct data *d)
{
	struct static_queue *q = d->data_type_pnt;

	if (static_queue_is_empty(d) == FALSE) {
		q->arr[(q->front++) % MAX_QUEUE_SIZE] = NULL;
		
		return TRUE;
	}

	return FALSE;
}

int static_queue_print(struct data *d, int flag)
{
	struct static_queue *q = (d->data_type_pnt);
	FILE *f;
	int cnt = (q->front % MAX_QUEUE_SIZE);
	
	if (flag == TO_FILE) {
		if (d->filename_download) { 
			f = fopen(d->filename_download, "w");
			if (!f) {
				printf("File opening error\n");
				return FALSE;
			}
			do {
				if (q->arr[cnt]) 
					fprintf(f, "%s\n", (char*)q->arr[cnt]);
				cnt = (cnt + 1) % MAX_QUEUE_SIZE;
			} while(cnt != ((q->rear + 1) % MAX_QUEUE_SIZE));

			fclose(f);

			return TRUE;
		} else {
			printf("No download file specified!\n");

			return FALSE;
		}

		return FALSE;

	} else if (flag == TO_STDOUT) {
		do {
			if (q->arr[cnt])
				printf("[%s] ", (char*)q->arr[cnt]);
			else
				printf("[null] ");
			cnt = (cnt + 1) % MAX_QUEUE_SIZE;
		} while(cnt != ((q->rear + 1) % MAX_QUEUE_SIZE));
		printf("\n\n======================================\n");

		return TRUE;
	}

	return FALSE;
}

int static_queue_upload(struct data *d)
{
	FILE *file;
	char line[256];

	if (d->filename_upload) {
		file = fopen(d->filename_upload, "r");
		if (file) {
			while (fgets(line, sizeof(line), file)) {
					//delete last \n symbol 
					line[strlen(line) - 1] = '\0';
					if (static_queue_push(d, strdup(line)) == FALSE) {
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

int static_queue_download(struct data *d)
{
	if (static_queue_is_empty(d) == TRUE) {
		printf("Not creating files for an empty queue\n");
		return FALSE;
	}

	return static_queue_print(d, TO_FILE);
}
