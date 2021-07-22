#include <stdio.h> 
#include <stdlib.h>
#include <getopt.h>
#include "stack.h"

int main(int argc, char *argv[])
{
	static int create_stack_flag;
	static int create_heap_flag;
	int c;

	while (1) {
		static struct option long_options[] =
		{
			{"create-stack", no_argument, 0, 'd'},
			{"create-heap", no_argument, 0, 'e'},
			{"push", required_argument, 0, 'a'},
			{"pop", no_argument, 0, 'b'},
			{"data-type", required_argument, 0, 'c'},
			{"file", required_argument, 0, 'f'},
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
			printf("option push selected with : %s\n", optarg);
			break;
		case 'b':
			printf("option pop selected\n");
			break;
		case 'c':
			printf("option data type selected with : %s\n", optarg);
			break;
		case 'd':
			printf("stack creation\n");
			break;
		case 'e':
			printf("heap creation\n");
			break;
		case 'f':
			printf("file creation selected with value : %s\n", optarg);
			break;
		case '?':
			break;
		default:
			abort();
		}


	}

	return 0; 
}
