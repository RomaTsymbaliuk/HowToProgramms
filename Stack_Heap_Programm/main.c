#include <stdio.h> 
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
	static int create_stack_flag;
	static int create_heap_flag;
	int c;

	while (1) {
		static struct option long_options[] =
		{
			{"create stack", no_argument, &create_stack_flag, 1},
			{"create heap", no_argument, &create_heap_flag, 1},
			{"push", required_argument, 0, 'a'},
			{"pop", no_argument, 0, 'b'},
			{"data-type", required_argument, 0, 'c'},
			{"file", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long(argc, argv, "abc:f", long_options, &option_index);
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
			puts("option push selected");
			break;
		case 'b':
			puts("option pop selected");
			break;
		case 'c':
			puts("option data type selected");
			break;
		case 'f':
			printf("file creation selected with value : %s", optarg);
			break;
		case '?':
			break;
		default:
			abort();
		}


	}

	return 0; 
}
