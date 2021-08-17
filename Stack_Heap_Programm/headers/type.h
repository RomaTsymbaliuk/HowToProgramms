#ifndef __TYPE_H
#define __TYPE_H

#define TRUE 0
#define FALSE 1
#define MAX_STACK_SIZE 100 
#define MAX_QUEUE_SIZE 100

enum {STATIC_ARR_STACK, DYNAMIC_ARR_STACK, LIST_STACK, STATIC_ARR_QUEUE, DYNAMIC_ARR_QUEUE, LIST_QUEUE};

/* Print */
enum {TO_FILE, TO_STDOUT};

struct data {
	void *data_type_pnt;
	int data_type;
	int size;
	char *filename_upload;
	char *filename_download;
	int (*init)(struct data *d);
	int (*push)(struct data *d, void *data);
	int (*pop)(struct data *d);
	int (*print)(struct data *d, int flag);
	int (*download)(struct data *d);
	int (*upload)(struct data *d);
};


#endif /* __TYPE_H  */
