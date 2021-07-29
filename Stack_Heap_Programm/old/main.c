#include <stdio.h> 
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "structures.h"
#include "data.h"
#define CMD_NUM 40

struct stack *st;
struct queue *q;
struct commands{
	int (*push_pointer)(int flag_data_type, char *str);
	int (*pop_pointer)(int flag_data_type);
	char *str;
};

int main(int argc, char *argv[])
{
	static int create_stack_flag;
	static int create_queue_flag;
	int c;
       	int status;
	enum {STATIC_ARRAY, DYNAMIC_ARRAY, LINKED_LIST};
	int data_type = -1;
	int size = 0;
	struct commands *cmd = (struct commands*)malloc(sizeof(struct commands) * CMD_NUM);
	int file_flag = 0;
	char *filename;

	for (int i = 0; i < CMD_NUM; i++){
		cmd[i].push_pointer = NULL;
		cmd[i].pop_pointer = NULL;
	}
	int k = 0;

	while (1) {

		static struct option long_options[] =
		{
			{"create-stack", required_argument, 0, 'd'},
			{"create-queue", required_argument, 0, 'e'},
			{"push", required_argument, 0, 'a'},
			{"pop", no_argument, 0, 'b'},
			{"data-type", required_argument, 0, 'c'},
			{"file", required_argument, 0, 'f'},
			{"print", no_argument, 0, 'p'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long(argc, argv, "d:f:c:a:be", long_options, &option_index);
		if (c == -1){
			break;
		}

		switch (c) {
		case 0:
			if (long_options[option_index].flag != 0){
				break;
			}
			printf("option %s", long_options[option_index].name);
			if (optarg){
				printf("with arg %s", optarg);
			}
			printf("\n");
			break;
		
		case 'a':
			if (create_stack_flag)
				cmd[k].push_pointer = stack_push;
			else if (create_queue_flag)
				cmd[k].push_pointer = queue_push;
			cmd[k].pop_pointer = NULL;
			cmd[k].str = optarg;
			printf("CASE : cmd[%d].str -  %s\n", k, cmd[k].str);
			k++;
			break;
		case 'b':
			cmd[k].push_pointer = NULL;
			cmd[k].pop_pointer = stack_pop;
			cmd[k].str = NULL;
			k++;
			break;
		case 'c':
			if (!strcmp(optarg, "dynamic-arr")){
				data_type = DYNAMIC_ARRAY;
			} else if (!strcmp(optarg, "static-arr")) {
				data_type = STATIC_ARRAY;
			} else if (!strcmp(optarg, "linked-list")) {
				data_type = LINKED_LIST;
			}
			break;
		case 'd':
			create_stack_flag = 1;
			if (optarg){
				printf("optarg : %s", optarg);
				size = atoi(optarg);
			}
			break;
		case 'e':
			create_queue_flag = 1;
			if (optarg){
				size = atoi(optarg);
			}
			break;
		case 'f':
			file_flag = 1;
			filename = optarg;
			break;
		case 'p':
			
			break;
		case '?':
			break;
		default:
			abort();
		}


	}
	if (create_stack_flag) {

		st = (struct stack *)malloc(sizeof(struct stack));
		int status = stack_init(data_type, size);
		for (int i = 0; i < k; i++){
			if (cmd[i].push_pointer) {
//				printf("in main : cmd[%d].str %s\n", i, cmd[i].str);
				cmd[i].push_pointer(data_type, cmd[i].str);
			} else if (cmd[i].pop_pointer) {
				cmd[i].pop_pointer(data_type);
			}
		}
		stack_print(data_type, file_flag, filename);
	} else if (create_queue_flag) {
		q = (struct queue *)malloc(sizeof(struct queue));
		int status = queue_init(data_type, size);
		for (int i = 0; i < k; i++) {
			if (cmd[i].push_pointer) {
				cmd[i].push_pointer(data_type, cmd[i].str);
			} else if (cmd[i].pop_pointer) {
				cmd[i].pop_pointer(data_type);
			}
		}
		queue_print(data_type, file_flag, filename);

	}
	return 0; 
}
