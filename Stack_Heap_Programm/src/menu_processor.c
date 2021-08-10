#include "menu_processor.h"
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

struct cmd_data *process_user_input(int argc, char *argv[])
{

	int c;
	int size;
        struct cmd_data *cm_d = (struct cmd_data*)malloc(sizeof(struct cmd_data));
        struct cmd **cm = (struct cmd **)malloc(sizeof(struct cmd *) * CMD_NUMBER);
        struct data *d;
        
        for (int j = 0; j < CMD_NUMBER; j++) {
                cm[j] = NULL;
        }
        int i = 0;

        while (1) {

                static struct option long_options[] =
                {
                        {"create-static-stack", required_argument, 0, 'd'},
                        {"create-static-queue", required_argument, 0, 'e'},
                        {"create-dynamic-stack", required_argument, 0, 'x'},
                        {"create-dynamic-queue", required_argument, 0, 'y'},
                        {"create-list-stack", no_argument, 0, 'q'},
                        {"create-list-queue", required_argument, 0, 'w'},
                        {"push", required_argument, 0, 'a'},
                        {"pop", no_argument, 0, 'b'},
                        {"file", required_argument, 0, 'f'},
			{"upload", no_argument, 0, 'u'},
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
			if (optarg) {
                                if (strlen(optarg) > MAX_STR_LEN) {
                                        printf("Too big string! \n");
                                        return NULL;
                                }
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd));
                        cm[i]->cmd_type = PUSH;
                        cm[i]->user_data = optarg;
                        cm[i]->size = 0;
                        i++;
			}
                        break;
                case 'b':
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd));
                        cm[i]->cmd_type = POP;
                        cm[i]->user_data = NULL;
                        cm[i]->size = 0;
                        i++; 
                        break;
                case 'p':
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd));
                        cm[i]->cmd_type = PRINT;
                        cm[i]->user_data = NULL;
                        cm[i]->size = 0;
                        i++; 
                        break;

                case 'd':
                        if (optarg){
                                size = atoi(optarg);
                                if (size > MAX_SIZE || size < MIN_SIZE) {
                                        printf("NOT ALLOWED SIZE : %d\n", size);
                                        return NULL;
                                }
                                d = &s_stack_obj;
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
                                if (size > MAX_SIZE || size < MIN_SIZE) {
                                        printf("NOT ALLOWED SIZE : %d\n", size);
                                        return NULL;
                                }
                                d = &d_stack_obj;
                                d->size = size;
                        }
                        break;
                case 'y':
                        if (optarg){
                                size = atoi(optarg);
                        }
                        break;
                case 'q':

                        d = &l_stack_obj;
                        break;
                case 'w':
                        if (optarg){
                                size = atoi(optarg); 
                        }
                        break;
		case 'u':
			break;
                case 'f':
                        d->filename = optarg;
                        break;
                case '?':
                        break;
                default:
                        abort();
                        return NULL;
                }

        }

        cm_d->d = d;
        cm_d->commands = cm;

        return cm_d;
}

int run_user_cmd(struct cmd_data *cm_d)
{
        
        int i = 0;
        struct data *d = cm_d->d;
        struct cmd **cm = cm_d->commands; 
        d->init(d);
        d->upload(d);
        while (cm[i]) {
                switch(cm[i]->cmd_type){
                        case PUSH:
                                d->push(d, cm[i]->user_data);
                                break;
                        case POP:
                                d->pop(d);
                                break;
                        case PRINT:
                                d->print(d, TO_STDOUT);
                                break;
                        default:
                                printf("Unrecognized function. Abort");
                                return FALSE;
                }
                i = i + 1;
        }
        d->download(d);
        return TRUE;
}