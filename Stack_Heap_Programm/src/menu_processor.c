#include "menu_processor.h"
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

static void help(void)
{
        printf(HELP_MENU);
}

struct cmd_data *process_user_input(int argc, char *argv[])
{

	int c;
	int size;
        int option_index;
        int i = 0;
        static struct cmd_data cm_d_obj;
        struct cmd_data *cm_d = &cm_d_obj;
        struct data *d;
        static struct cmd *cm[CMD_NUMBER] = {NULL};
        static struct option long_options[] =
        {
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
                {"sleep", no_argument, 0, 's'},
                {0, 0, 0, 0}
        };

        if (!cm) {
                printf("Memory leak\n");
                return NULL;
        }
        /*if (!cm_d) {
                printf("Memory leak\n");
                return NULL;
        }*/
        for (int i = 0; i < CMD_NUMBER; i++) {
                cm[i] = NULL;
        }
        while (1) { 

                if (i > CMD_NUMBER) {
                        printf("Too many commands ! Max commands number : %d\n", CMD_NUMBER);
                        return NULL;
                }

                option_index = 0;
                c = getopt_long(argc, argv, "d:f:c:a:bes", long_options, &option_index); // recheck
                if (c == -1){
                        break;
                        return NULL;

                }

                switch (c) {
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
                        d = &s_queue_obj;
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
                                if (size > MAX_STACK_SIZE || size < MIN_STACK_SIZE) {
                                        printf("NOT ALLOWED SIZE : %d\n", size);
                                        return NULL;
                                }
                                d = &d_queue_obj;
                                d->size = size;
                        } else {
                                printf("Arg required");
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
                case 's':
                        cm[i] = (struct cmd*)malloc(sizeof(struct cmd)); 
                        if (cm[i]) {
                                cm[i]->cmd_type = SLEEP;
                                i++;

                        } else {
                                printf("Memory leak \n");
                                return NULL;
                        }
                        break;
                case '?':
                case 'h':
                default:
                        printf("\nFFFFFFFFFFFFFFFFFF %x\n", c);
                        help();
                        return NULL;
                }

        }

        if ( i != 0) {
                cm_d->d = d;
                cm_d->commands = cm;
        } else {
                
                help();
                printf("\nNo options specified. Abort\n");
                return NULL;
        }

        return cm_d; 
}

int run_user_cmd(struct cmd_data *cm_d)
{
        
        int i = 0;
        int res = TRUE;
        struct data *d = cm_d->d;
        struct cmd **cm = cm_d->commands; 
        int status;

        if (d->init(d) == FALSE) {
                printf("Init crashed\n");
                return FALSE;
        }
        if (d->upload(d) == FALSE) {
                printf("upload crashed, it will be empty\n");
                res = FALSE;
        }

        while (cm[i]) { 

                if (i > CMD_NUMBER) {
                        printf("To much commands number %d\n", CMD_NUMBER);
                        res = FALSE;
                        break;
                }

                switch(cm[i]->cmd_type){
                        case PUSH:
                                if (d->push(d, cm[i]->user_data) == FALSE) {
                                        printf("Push crashed\n");
                                        res = FALSE;
                                }
                                break;
                        case POP:
                                if (d->pop(d) == FALSE) {
                                        printf("Pop crashed\n");
                                        res = FALSE;
                                } 
                                break;
                        case PRINT:
                                if (d->print(d, TO_STDOUT) == FALSE) {
                                        printf("Print crashed\n");
                                        res = FALSE;
                                }
                                break;
                        case SLEEP:
                                sleep(100000000);
                        default:
                                printf("Unrecognized option. Abort");
                                res = FALSE;
                }
                i = i + 1;
        }
        while (i-- && cm[i]) {
                free(cm[i]);
        }
        if (d->download(d) == FALSE) {
                printf("Download crashed\n");
                res = FALSE;
        }

        return res;
}