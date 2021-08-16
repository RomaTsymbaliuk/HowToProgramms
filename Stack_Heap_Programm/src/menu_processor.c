#include "menu_processor.h"
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

// TABI !!!!!!
struct cmd_data *process_user_input(int argc, char *argv[])
{

	int c;
	int size;
        int option_index;
        int i = 0;
        struct cmd_data *cm_d = (struct cmd_data*)malloc(sizeof(struct cmd_data)); // DE proverki????
        struct data *d;
        struct cmd **cm = (struct cmd**)malloc(sizeof(struct cmd*) * CMD_NUMBER);

        if (!cm) {
                printf("Memory leak\n");
                return NULL;
        }
        for (int i = 0; i < CMD_NUMBER; i++) {
                cm[i] = NULL;
        }

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
                {"file-upload", required_argument, 0, 'f'},
                {"file-download", required_argument, 0, 't'},
                {"upload", no_argument, 0, 'u'},
                {"print", no_argument, 0, 'p'},
                //help gde??? 
                {0, 0, 0, 0}
        };

        if (!cm_d) {
                return NULL;
        }

        while (1) { // Proverka na i > CMD_NUMBER

                if (i > CMD_NUMBER) {
                        printf("Too many commands ! Max commands number : %d\n", CMD_NUMBER);
                        return NULL;
                }

                option_index = 0;
                c = getopt_long(argc, argv, "d:f:c:a:be", long_options, &option_index); // recheck
                if (c == -1){
                        //help gde???
                        break;
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
                                cm[i] = (struct cmd*)malloc(sizeof(struct cmd)); //Proverka
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
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd)); //Proverka
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
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd)); //Proverka
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
                        if (optarg){
                                size = atoi(optarg);
                                if (size > MAX_SIZE || size < MIN_SIZE) {
                                        printf("NOT ALLOWED SIZE : %d\n", size);
                                        return NULL;
                                }
                                d = &s_stack_obj;
                        } else {
                                printf("Arg required\n");
                                return NULL;
                        }
                        break;
                case 'e':
                        if (optarg) {
                                size = atoi(optarg);
                        }
                        break;
                case 'x':
                        if (optarg) {
                                size = atoi(optarg);
                                if (size > MAX_SIZE || size < MIN_SIZE) {
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
                        //help
                        break;
                default:
                        //help
                        abort();
                        return NULL;
                }

        }

        if ( i != 0) {
                cm_d->d = d;
                cm_d->commands = cm;  
        } else {
                printf("No options specified\n");
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
 
        status = d->init(d);
        if (status == FALSE) {
                printf("Init crashed\n");
                return FALSE;
        }
        status = d->upload(d);
        if (status == FALSE) {
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
                                status = d->push(d, cm[i]->user_data);
                                if (status == FALSE) {
                                        printf("Push crashed\n");
                                        return FALSE;
                                }
                                break;
                        case POP:
                                status = d->pop(d);
                                if (status == FALSE) {
                                        printf("Pop crashed\n");
                                        return FALSE;
                                } 
                                break;
                        case PRINT:
                                status = d->print(d, TO_STDOUT);
                                if (status == FALSE) {
                                        printf("Print crashed\n");
                                        return FALSE;
                                }
                                break;
                        default:
                                printf("Unrecognized function. Abort");
                                return FALSE;
                }
                i = i + 1;
        }

        status = d->download(d);
        if (status == FALSE) {
                printf("Download crashed\n");
                return FALSE;
        }

        return TRUE;
}