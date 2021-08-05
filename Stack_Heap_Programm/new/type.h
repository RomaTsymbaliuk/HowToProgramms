#ifndef __TYPE_H
#define __TYPE_H
#define TRUE 1
#define FALSE 0
#define MAX_SIZE 100

enum {STATIC_ARR_STACK, DYNAMIC_ARR_STACK, LIST_STACK, STATIC_ARR_QUEUE, DYNAMIC_ARR_QUEUE, LIST_QUEUE};



struct data {
	
	struct {
		void *structure_pointer;
	} anon_s;
	
	int data_type;
	int size;
	void *data_ptr;
	char *filename;
	int (*init)(struct data *d);
	int (*push)(struct data *d);
	int (*pop)(struct data *d);
	int (*print)(struct data *d);
	int (*download)(struct data *d);
	int (*upload)(struct data *d);
};


#endif /* __TYPE_H  */
