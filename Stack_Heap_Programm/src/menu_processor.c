#include "menu_processor.h"
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

int help() 
{
        FILE *f;
        char line[256];

        f = fopen("help.txt", "r");
        if (!f) {
                printf("file help opening failed");
                return FALSE;
        } else {
                while (fgets(line, sizeof(line), f)) {
                        printf("%s", line);
                }
        fclose(f);
        return TRUE;      
        }

        return FALSE;
}

struct cmd_data *process_user_input(int argc, char *argv[])
{

	int c;
	int size;
        int option_index;
        int i = 0;
        struct cmd_data *cm_d = (struct cmd_data*)malloc(sizeof(struct cmd_data)); 
        struct data *d;
        struct cmd **cm = (struct cmd**)malloc(sizeof(struct cmd*) * CMD_NUMBER);
        static struct option long_options[] =
        {
                /*

                NAME
                        main - create stack programm

                SYNOPSIS
                        ./main [OPTIONS]

                DESCRIPTION
                        
                        --create-static-stack           create stack with static array data structure, no size specified

                        --create-dynamic-stack [%d]     create stack with dynamic array data structure, specify stack size 

                        --create-list-stack             create stack with linked list data structure

                        --push                          add an element to the stack

                        --pop                           pop an element from the stack

                        --print                         print the stack

                LIMITS
                        DYNAMIC STACK  SIZE             100 elements

                        STRING LENGTH                   50 symbols

                        OPTIONS NUMBER                  30 options

                */
                
                {"create-static-stack", no_argument, 0, 'd'},
                {"create-static-queue", no_argument, 0, 'e'},
                {"create-dynamic-stack", required_argument, 0, 'x'},
                {"create-dynamic-queue", required_argument, 0, 'y'},
                {"create-list-stack", no_argument, 0, 'q'},
                {"create-list-queue", required_argument, 0, 'w'},
                {"push", required_argument, 0, 'a'},
                {"pop", no_argument, 0, 'b'},
                {"file-upload", required_argument, 0, 'f'},
                {"file-download", required_argument, 0, 't'},
                {"print", no_argument, 0, 'p'},
                {0, 0, 0, 0}
        };

        if (!cm) {
                printf("Memory leak\n");
                return NULL;
        }
        if (!cm_d) {
                printf("Memory leak\n");
                return NULL;
        }
        for (int i = 0; i < CMD_NUMBER; i++) {
                cm[i] = NULL;
        }
        while (1) { 

                if (i > CMD_NUMBER) {
                        printf("Too many commands ! Max commands number : %d\n", CMD_NUMBER);
                        return NULL;
                }

                option_index = 0;
                c = getopt_long(argc, argv, "d:f:c:a:be", long_options, &option_index); // recheck

                
                if (c == -1){
                        
                        /*
                        if (help() == FALSE) {
                                printf("Help error!\n");
                                return NULL;
                        }
                        */
                        break;
                        return NULL;

                }
                switch (c) {
                case 0: // ??? 
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
                                        printf("Too big string! Max size is %d\n", MAX_STR_LEN);
                                        return NULL;
                                }
                                cm[i] = (struct cmd*)malloc(sizeof(struct cmd)); 
                                if (cm[i]) {
                                        cm[i]->cmd_type = PUSH;
                                        cm[i]->user_data = optarg;
                                        cm[i]->size = 0;
                                        i++;  
                                } else {
                                        printf("Memory leak \n");
                                        return NULL;
                                }
			} else {
                                printf("Push arg required\n");
                                return NULL;
                        }
                        break;
                case 'b':
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd)); 
                        if (cm[i]) {
                                cm[i]->cmd_type = POP;
                                cm[i]->user_data = NULL;
                                cm[i]->size = 0;
                                i++;

                        } else {
                                printf("Memory leak \n");
                                return NULL;
                        }
                        
                        break;
                case 'p':
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd)); 
                        if (cm[i]) {
                                cm[i]->cmd_type = PRINT;
                                cm[i]->user_data = NULL;
                                cm[i]->size = 0;
                                i++;  
                        } else {
                                printf("Memory leak \n");
                                return NULL;
                        }

                        break;

                case 'd':      
                        d = &s_stack_obj;
                        break;
                case 'e':
                        if (optarg) {
                                size = atoi(optarg);
                        }
                        break;
                case 'x':
                        if (optarg) {
                                size = atoi(optarg);
                                if (size > MAX_STACK_SIZE || size < MIN_STACK_SIZE) {
                                        printf("NOT ALLOWED SIZE : %d\n", size);
                                        return NULL;
                                }
                                d = &d_stack_obj;
                                d->size = size;
                        } else {
                                printf("Arg required \n");
                                return NULL;
                        }
                        break;
                case 'y':
                        if (optarg) {
                                size = atoi(optarg);
                        }
                        break;
                case 'q':
                        d = &l_stack_obj;
                        break;
                case 'w':
                        if (optarg) {
                                size = atoi(optarg); 
                        }
                        break;
		case 'u':
			break;
                case 'f':
                        d->filename_upload = optarg;
                        break;
                case 't':
                        d->filename_download = optarg;
                        break;
                case '?':      
                        /*
                        if (help() == FALSE) {
                                printf("Help error\n");
                                return NULL;
                        };
                        */
                        break;
                default:

                        if (help() == FALSE) {
                              return NULL;
                        }
                        return NULL;
                }

        }

        if ( i != 0) {
                cm_d->d = d;
                cm_d->commands = cm;  
        } else {
                
                if (help() == FALSE){
                        printf("Help error!\n");
                        return NULL;
                }

                printf("\nNo options specified. Abort\n");
                return NULL;
        }

        return cm_d; 
}

int run_user_cmd(struct cmd_data *cm_d)
{
        
        int i = 0;
        struct data *d = cm_d->d;
        struct cmd **cm = cm_d->commands; 
        int status;

        if (d->init(d) == FALSE) {
                printf("Init crashed\n");
                return FALSE;
        }
        if (d->upload(d) == FALSE) {
                printf("upload crashed\n");
                return FALSE;
        }

        while (cm[i]) { //Ochibka na 101 elenete

                if (i > CMD_NUMBER) {
                        printf("To much commands number %d\n", CMD_NUMBER);
                        return FALSE;
                }

                switch(cm[i]->cmd_type){
                        case PUSH:
                                if (d->push(d, cm[i]->user_data) == FALSE) {
                                        printf("Push crashed\n");
                                        return FALSE;
                                }
                                break;
                        case POP:
                                if (d->pop(d) == FALSE) {
                                        printf("Pop crashed\n");
                                        return FALSE;
                                } 
                                break;
                        case PRINT:
                                if (d->print(d, TO_STDOUT) == FALSE) {
                                        printf("Print crashed\n");
                                        return FALSE;
                                }
                                break;
                        default:
                                printf("Unrecognized option. Abort");
                                return FALSE;
                }
                i = i + 1;
        }
        if (d->download(d) == FALSE) {
                printf("Download crashed\n");
                return FALSE;
        }

        return TRUE;
}