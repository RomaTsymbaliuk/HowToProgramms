#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "serial.h"

void reserve_space(struct packet *b, size_t bytes)
{
	if((b->next + bytes) > b->size) {
		/* double size to enforce O(lg N) reallocs */
		b->data = realloc(b->data, b->size * 2);
		b->size *= 2;
	}
}

void serialize_int(int x, struct packet *b)
{
	x = htonl(x);
	reserve_space(b, sizeof(int));
	memcpy(((char *)b->data) + b->next, &x, sizeof(int));
	b->next += sizeof(int);
}

void serialize_menu(struct menu *input, struct packet *pkg)
{
	serialize_int(input->cmd_id, pkg);
	serialize_int(input->process_flags, pkg);
}