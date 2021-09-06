#ifndef SERIALIZE_H
#define SERIALIZE_H

#define INITIAL_SIZE 100

#include "menu_objs.h"

struct buffer {
	void *data;
	size_t next;
	size_t size;
};


struct buffer *new_buffer();
void reserve_space(struct buffer *b, size_t bytes);
void serialize_int(int x, struct buffer *b);
void serialize_menu(struct menu *input, struct buffer *buff);

#endif