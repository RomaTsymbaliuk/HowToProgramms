#ifndef PACKET_H
#define PACKET_H

#include <stddef.h>

#define INITIAL_SIZE 100

struct packet {
	size_t next;
	size_t size;
	void *data;
};

struct packet *new_packet();

#endif
