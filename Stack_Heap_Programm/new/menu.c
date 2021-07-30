#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif
#ifndef STDLIB
#define STDLIN
#include <stdlib.h>
#endif
#ifndef DATA_H
#define DATA_H
#include "data.h"
#endif
#ifndef MENU_H
#define MENU_H
#include "menu.h"
#endif
#ifndef GETOPT
#define GETOPT
#include <getopt.h>
#endif
#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif
#ifndef EXECTR
#define EXECTR
#include "executor.h"
#endif

struct data *menu(int argc, char *argv[], struct data *d)
{
	char *filename;
	int c;
	int size;
	d = (struct data*)malloc(sizeof(struct data));
	while (1) {

                static struct option long_options[] =
                {
                        {"create-stack", required_argument, 0, 'd'},
                        {"create-queue", required_argument, 0, 'e'},
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
			printf("Push choosen\n");
                        break;
                case 'b':
			printf("Pop chosen\n");
                        break;
                        break;
                case 'c':
                       if (!strcmp(optarg, "dynamic-arr")){
                                printf("Data type dynamic chosen\n");
				d->data_type = DYNAMIC_ARRAY;
                        } else if (!strcmp(optarg, "static-arr")) {
				d->data_type = STATIC_ARRAY;
                                printf("Data type static chosen\n");
                        } else if (!strcmp(optarg, "linked-list")) {
				d->data_type = LINKED_LIST;
                                printf("Data type linked-list chosen\n");
                        }
                        break;
                case 'd':
                        if (optarg){
                                size = atoi(optarg);
				printf("Stack choosen with size : %d!\n", size);
				d->structure_size = size;
				d->init = init;
				d->data_structure = STACK;
				d->push = NULL;
				d->pop = NULL;
				d->print = NULL;
                        }
                        break;
                case 'e':
                        if (optarg){
				
                                size = atoi(optarg);
				printf("Queue chosen with size : %d\n", size);
				d->structure_size = size;
				d->init = init;
				d->data_structure = QUEUE;
				d->push = NULL;
				d->pop = NULL;
				d->print = NULL;
                        }
                        break;
                case 'f':
                        filename = optarg;
                        break;
                case 'p':
			d->init = NULL;
			d->push = NULL;
			d->pop = NULL;
			d->print = print;
                        break;
                case '?':
                        break;
                default:
                        abort();
                }
	
        }
	executor(d);
}
