#include "menu_processor.h"
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

struct data *choice = NULL;

struct cmd **process_user_input(int argc, char *argv[])
{
        
	
	int c;
	int size;
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
                                cm[i] = (struct cmd*)malloc(sizeof(struct cmd));
                                cm[i]->d = (struct data*)malloc(sizeof(struct data));
                                cm[i]->d = d;
                                cm[i]->fn = (cm[i]->d)->push;
                                cm[i]->user_data = optarg;
                                i++;
			}
                        break;
                case 'b':
                        d->data_ptr = NULL;
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd));
                        cm[i]->d = d;
                        cm[i]->fn = (cm[i]->d)->pop;
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
                                d->size = size;
                                cm[i] = (struct cmd*)malloc(sizeof(struct cmd));
                                cm[i]->d = (struct data*)malloc(sizeof(struct data));
                                cm[i]->d = d;
                                cm[i]->user_data = NULL;
                                cm[i]->fn = (cm[i]->d)->init;
                                i++;
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
                                cm[i] = (struct cmd*)malloc(sizeof(struct cmd));
                                cm[i]->d = (struct data*)malloc(sizeof(struct data));
                                cm[i]->d = d;
                                cm[i]->user_data = NULL;
                                cm[i]->fn = (cm[i]->d)->init;
                                i++;
                        }
                        break;
                case 'y':
                        if (optarg){
                                size = atoi(optarg);
                        }
                        break;
                case 'q':

                        d = &l_stack_obj;
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd));
                        cm[i]->d = (struct data*)malloc(sizeof(struct data));
                        cm[i]->d = d;
                        cm[i]->user_data = NULL;
                        cm[i]->fn = (cm[i]->d)->init;
                        i++;
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
                case 'p':
                        d->data_ptr = NULL;
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd));
                        cm[i]->d = d;
                        cm[i]->fn = (cm[i]->d)->print;
                        i++;
                        break;
                case '?':
                        break;
                default:
                        abort();
                }

        }

        return cm;
}

void run_user_cmd(struct cmd **cm)
{
        int i = 0;
        while (cm[i]) {
                (cm[i]->d)->data_ptr = cm[i]->user_data;
                cm[i]->fn(cm[i]->d);
                i = i + 1;
        }
}