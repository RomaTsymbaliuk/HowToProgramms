#include "packet.h"
#include <stdlib.h>


struct packet *new_packet() 
{
	struct packet *b = malloc(sizeof(struct packet));

	b->data = malloc(INITIAL_SIZE);
	if (!b->data) {
		return NULL;
	}
	b->size = INITIAL_SIZE;
	b->next = 0;
	
	return b;
}