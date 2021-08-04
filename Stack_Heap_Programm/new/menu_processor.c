#include "menu_processor.h"
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

struct data *choice = NULL;

struct data *process_user_input(int argc, char *argv[])
{
        
	char *filename;
	int c;
	int size;
	struct data *d = (struct data*)malloc(sizeof(struct data));
        struct data **objs = (struct data**)malloc(sizeof(struct data) * 6);
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
                                if (strlen(optarg) > MAX_STR_LEN) {
                                        printf("Too big string! \n");
                                        return NULL;
                                }
		  		d->data_ptr = optarg;
			}
                        break;
                case 'b':
                        break;
                case 'd':
                        if (optarg){
                                size = atoi(optarg);
                                if (size > MAX_SIZE || size < MIN_SIZE) {
                                        printf("NOT ALLOWED SIZE : %d\n", size);
                                        return NULL;
                                }
                                d = objs[STATIC_ARR_STACK];
                                d->size = size;
                        }
                        break;
                case 'e':
                        if (optarg){
                                size = atoi(optarg);
                        }
                        break;
                case 'x':
                        if (optarg){
                                size = atoi(optarg);
                        }
                        break;
                case 'y':
                        if (optarg){
                                size = atoi(optarg);
                        }
                        break;
                case 'q':
                        if (optarg){
                                size = atoi(optarg);
        
                        }
                        break;
                case 'w':
                        if (optarg){
                                
                                size = atoi(optarg);
                                
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

        return d;
}

struct data *run_user_cmd(struct data *choice)
{
        choice->init(choice);
        choice->push(choice);
        choice->print(choice);
}