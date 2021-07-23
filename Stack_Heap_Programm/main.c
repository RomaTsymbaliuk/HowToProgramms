#include <stdio.h> 
#include <stdlib.h>
#include <getopt.h>
#include "stack.h"

int main(int argc, char *argv[])
{
	static int create_stack_flag;
	static int create_heap_flag;
	int c;
        struct stack *st;
	int status;
	enum {STATIC_ARRAY, DYNAMIC_ARRAY, LINKED_LIST};
	int data_type = -1;
	while (1) {
		static struct option long_options[] =
		{
			{"create-stack", required_argument, 0, 'd'},
			{"create-heap", no_argument, 0, 'e'},
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
			status = stack_push(st, optarg);
			break;
		case 'b':
			stack_pop(st);
			break;
		case 'c':

			printf("option data type selected with : %s\n", optarg);
			if (optarg == "dynamic-arr"){
				data_type = DYNAMIC_ARRAY;
			} else if (optarg == "static-arr") {
				data_type = STATIC_ARRAY;
			} else if (optarg == "linked-list") {
				data_type = LINKED_LIST;
			}
			break;
		case 'd':
			printf("stack creation\n");
			int size = atoi(optarg);
			printf("optarg : %d\n", size);
			status = stack_init(st, DYNAMIC_ARRAY, size);
			printf("Operation status : %d\n", status);
			break;
		case 'e':
			printf("queue creation\n");
			break;
		case 'f':
			printf("file creation selected with value : %s\n", optarg);
			break;
		case 'p':
			stack_print(st);
			break;
		case '?':
			break;
		default:
			abort();
		}


	}

	return 0; 
}
