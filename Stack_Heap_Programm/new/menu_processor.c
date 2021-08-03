#include "menu_processor.h"
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

struct obj *choice = NULL;

struct obj *process_user_input(int argc, char *argv[])
{
        
	char *filename;
	int c;
	int size;
	struct data *d = (struct data*)malloc(sizeof(struct data));
        struct obj **objs = (struct obj**)malloc(sizeof(struct obj) * 6);
        objs[STATIC_ARR_STACK] = &s_stack_obj;
        

        while (1) {

                static struct option long_options[] =
                {
                        {"create-static-stack", required_argument, 0, 'd'},
                        {"create-static-queue", required_argument, 0, 'e'},
                        {"create-dynamic-stack", required_argument, 0, 'x'},
                        {"create-dynamic-queue", required_argument, 0, 'y'},
                        {"create-list-stack", required_argument, 0, 'q'},
                        {"create-list-queue", required_argument, 0, 'w'},
                        {"push", required_argument, 0, 'a'},
                        {"pop", no_argument, 0, 'b'},
                        {"file", required_argument, 0, 'f'},
                        {"print", no_argument, 0, 'p'},
			{"upload", no_argument, 0, 'u'},
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
			if (optarg) {
		  		printf("Push choosen\n");
				
			}
                        break;
                case 'b':
		
			printf("Pop chosen\n");
                        break;
                case 'd':
                        if (optarg){
                                size = atoi(optarg);
				printf("Static Stack choosen with size : %d!\n", size);
                                choice = objs[STATIC_ARR_STACK];
                                printf("Choice row : %s", choice->row);
                                return choice;
                        }
                        break;
                case 'e':
                        if (optarg){
				
                                size = atoi(optarg);
				printf("Static Queue chosen with size : %d\n", size);
				
                        }
                        break;
                case 'x':
                        if (optarg){
                                size = atoi(optarg);
                                printf("Dynamic Stack choosen with size : %d!\n", size);
        
                        }
                        break;
                case 'y':
                        if (optarg){
                                
                                size = atoi(optarg);
                                printf("Dynamic Queue chosen with size : %d\n", size);
                                
                        }
                        break;
                case 'q':
                        if (optarg){
                                size = atoi(optarg);
                                printf("List Stack choosen with size : %d!\n", size);
        
                        }
                        break;
                case 'w':
                        if (optarg){
                                
                                size = atoi(optarg);
                                printf("List Queue chosen with size : %d\n", size);
                                
                        }
                        break;
		case 'u':
			break;
                case 'f':
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


}

struct data *run_user_cmd(struct obj *choice)
{
        struct data *d;
}