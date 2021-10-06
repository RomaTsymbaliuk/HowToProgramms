#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include "executor.h"

struct list{
	char *item;
	struct list *next;
};

char *client_executor(char *cmd_data)
{
	FILE *fp;
	char result[1024] = {0};
	long int cmd_res_size = 0;
	char *cmd_to_ret;
	int i = 1;
	int size = 0;
	int mem_cnt = 0;
	int k = 0;
	int s = 0;
	struct list *head, *temp, *copy_head;
	char *final_cmd;

	head = malloc(sizeof(struct list));
	if (!head) {
		printf("Memory problem\n");
		return NULL;
	}

	copy_head = head;

	printf("CMD_DATA: %s-----\n", cmd_data);
	printf("-----GOT HERE 1-----\n");

	fp = popen(cmd_data, "r");
	if (fp == NULL) {
		printf("Failed to run command!\n");
		printf("\n---->%s<----\n", strerror(errno));
		return NULL;
	}

	printf("-----GOT HERE 2wwww-----\n");


	while (fgets(result, 1024, fp) != NULL) {

		head->item = strdup(result);
		if (!(head->item)) {
			printf("Malloc problem\n");
			return NULL;
		}
		head->next = malloc(sizeof(struct list));
		if (!(head->next)) {
			printf("Malloc problem\n");
			return NULL;
		}
		head = head->next;
		s++;
		size = size + strlen(result);

	}


	printf("GOT TO THE END , the end element is %s\n", head->item);

	printf("\n---->%s<----\n", strerror(errno));
	printf("---------GOT HERE 3----------\n");
	head->next = NULL;
	printf("Exit code: %i\n", WEXITSTATUS(pclose(fp)));

	printf("size of response : %d\n", size);
	cmd_to_ret = malloc(size);
	if (!cmd_to_ret) {
		printf("Memory problem\n");
		return NULL;
	}

	k = 0;
	for (temp = copy_head; temp->next != NULL; temp = temp->next)
	{
		if (temp->item) {
			for (int j = 0; j < strlen(temp->item); j++) {
				cmd_to_ret[k] = (temp->item)[j];
				k++;
			}
		}
	}
	printf("CMD TO RET LENGTH : %d\n", strlen(cmd_to_ret));

	printf("\n-----------There are %d elements here------------\n", s);
	while (copy_head->next != NULL) {
		temp = copy_head;
		copy_head = copy_head->next;
		if (temp->item) {
			free(temp->item);
		}
		if (temp) {
			free(temp);
			s--;
		}
	}
	printf("\n-----------There are %d elements here------------\n", s);


	if (head) {
		printf("Head not deleted\n");
		free(head);
	} else {
		printf("Head already deleted\n");
	}
	printf("HERE PROBLEM ?????____\n");

	if (head) {
		head = NULL;
	}
	if (temp) {
		temp = NULL;
	}
	if (copy_head) {
		copy_head = NULL;
	}

	return cmd_to_ret;

}