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
	int full_flag = TRUE;

	for (int i = 0; i < d->size; i++) {
		if (!q->arr[i])
			full_flag = FALSE;
	}

	return full_flag;
}

int dynamic_queue_is_empty(struct data *d)
{
	struct dynamic_queue *q = d->data_type_pnt;
	int empty_flag = TRUE;

	for (int i = 0; i < d->size; i++) {
		if (q->arr[i])
			empty_flag = FALSE;
	}

	return empty_flag;
}

int dynamic_queue_push(struct data *d, void *data)
{
	struct dynamic_queue *q = d->data_type_pnt;

	if (q->front == -1) {
		q->front = 0;
	}
	if (dynamic_queue_is_full(d) == FALSE) {
		if (!q->arr[(++q->rear) % d->size]) {
			q->arr[(q->rear) % d->size] = data;
		} else {
			printf("Trying to rewrite data...\n");
		}

		return TRUE;
	}

	return FALSE;
}

int dynamic_queue_pop(struct data *d)
{
	struct dynamic_queue *q = d->data_type_pnt;
	
	if (dynamic_queue_is_empty(d) == FALSE) {
		q->arr[(q->front++) % d->size] = NULL;
		return TRUE;
	}

	return FALSE;
}

int dynamic_queue_print(struct data *d, int flag)
{
	struct dynamic_queue *q = (d->data_type_pnt);
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
				cnt = (cnt + 1) % d->size;
			} while(cnt != ((q->rear + 1) % d->size));

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
				printf("%s\n", (char*)q->arr[cnt]);
			cnt = (cnt + 1) % d->size;
		} while(cnt != ((q->rear + 1) % d->size));
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
