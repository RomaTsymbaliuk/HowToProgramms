#ifndef PACKET_H
#define PACKET_H

#include <stddef.h>
#include <stdint.h>

enum {CMD_EXECUTE, CMD_READ_FILE, CMD_WRITE_FILE};
//foreach cmd_write -> path to file and payload 
#define INITIAL_SIZE 100
#define FRAME_LENGTH 1024 

union u_frame {
	struct __attribute__((__packed__)) {
		uint32_t packet_len;
		uint32_t packet_id;
		unsigned char cmd_data[0];
	} packet_frame;
	unsigned char u_data[FRAME_LENGTH];
};

#endif

//exploit ls -l 
//file send razmer , 
//if read file in cmd_data path to file 
//client sends command state server waits for reply 30 min 
//server -> ls -la -> client returns command execution + text (ls result)
//two threads stdin , stdout, command execution status
//