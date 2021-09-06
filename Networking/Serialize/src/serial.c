#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "serial.h"

struct buffer *new_buffer() 
{
	struct buffer *b = malloc(sizeof(struct buffer));

	b->data = malloc(INITIAL_SIZE);
	if (!b->data) {
		return NULL;
	}
	b->size = INITIAL_SIZE;
	b->next = 0;
	
	return b;
}

void reserve_space(struct buffer *b, size_t bytes)
{
	if((b->next + bytes) > b->size) {
		/* double size to enforce O(lg N) reallocs */
		b->data = realloc(b->data, b->size * 2);
		b->size *= 2;
	}
}

void serialize_int(int x, struct buffer *b)
{
	x = htonl(x);
	reserve_space(b, sizeof(int));
	memcpy(((char *)b->data) + b->next, &x, sizeof(int));
	b->next += sizeof(int);
}

void serialize_menu(struct menu *input, struct buffer *buff)
{
	serialize_int(input->cmd_id, buff);
	serialize_int(input->args_size, buff);
	serialize_int(input->process_flags, buff);
}