#ifndef ENUM_H
#define ENUM_H
enum {STATIC_ARRAY, DYNAMIC_ARRAY, LINKED_LIST, STACK, QUEUE};
#endif
//struct {
//	void *structure_pointer;
//} dat_struct_p;
struct {
	void *structure_pointer;
}data_structure_pointer;

struct data {
	int data_type;
	int data_structure;
	void *data_p;
	int (*push)(struct data *d);
	int (*pop)(struct data *d);
	int (*init)(struct data *d);
	void (*print)(struct data *d);
	struct 
	{
		void *str_pnt;
	} anon_s;
};

int push(struct data *d);
int pop(struct data *d);
int init(struct data *d);
void print(struct data *d);
