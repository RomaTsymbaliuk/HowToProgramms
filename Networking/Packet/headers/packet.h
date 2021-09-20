#ifndef PACKET_H
#define PACKET_H

#include <stddef.h>
#include <stdint.h>

enum {CMD_EXECUTE, CMD_READ_FILE, CMD_WRITE_FILE};
//foreach cmd_write -> path to file and payload 
#define INITIAL_SIZE 100
#define DATA_LENGTH 1024 // will be more than 50 in case files 

struct packet_header {
	uint32_t packet_id;
	uint32_t packet_len;
} __attribute__((__packed__));

struct packet_field {
	uint32_t cmd_id;
	uint32_t cmd_len;
	char cmd_data[1024];
	//send only stroka change to dynamic char *cmd_data
} __attribute__((__packed__));

struct packet_frame {
	struct packet_header header;
	struct packet_field fields;
} __attribute__((__packed__));

union u_frame {
	struct packet_frame pkt;
	char u_data[0];
};

#endif

//exploit ls -l 
//file send razmer , 
//if read file in cmd_data path to file 
//client sends command state server waits for reply 30 min 
//server -> ls -la -> client returns command execution + text (ls result)
//two threads stdin , stdout, command execution status
//