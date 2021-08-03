#ifndef DATA_H
#define DATA_H

struct data {
	int data_type;
	int data_structure;
	void *act_data;
	void **data_p;
	int structure_size;
	struct 
	{
		void *structure_pointer;
	} anon_s;

};

int push(struct data *d);
int pop(struct data *d);
int init(struct data *d);
void print(struct data *d);

#endif