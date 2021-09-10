#ifndef PACKET_H
#define PACKET_H

#include <stddef.h>
#include <stdint.h>

#define INITIAL_SIZE 100
#define DATA_LENGTH 50

struct packet_header {
	uint32_t packet_id;
	uint32_t packet_len;
} __attribute__((__packed__));

struct packet_frame {
	struct packet_header header;
	uint32_t cmd_id;
	uint32_t process_flag;
} __attribute__((__packed__));

#endif
