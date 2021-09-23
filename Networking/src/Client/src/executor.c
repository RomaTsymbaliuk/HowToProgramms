#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "executor.h"

struct list{
	char *item;
	struct list *next;
};

char *list_free()
{
	printf("Free function stuff\n");
}

char *client_executor(char *cmd_data)
{
	FILE *fp;
	char result[1024];
	long int cmd_res_size = 0;
	char *cmd_to_ret;
	int i = 1;
	int size = 0;
	int mem_cnt = 0;
	struct list *head, *temp, *copy_head;

	head = malloc(sizeof(struct list));
	if (!head) {
		return NULL;
	}

	copy_head = head;

	fp = popen(cmd_data, "r");
	if (fp == NULL) {
		printf("Failed to run command!\n");
		return NULL;
	}

	while (fgets(result, sizeof(result), fp) != NULL) {
		head->item = strdup(result);
		head->next = malloc(sizeof(struct list));
		head = head->next;
		size = size + strlen(result);
	}
	head->next = NULL;
	pclose(fp);

	cmd_to_ret = malloc(size * sizeof(char));
	if (!cmd_to_ret) {
		printf("Memory problem\n");
		return NULL;
	}
	int k = 0;
	for (temp = copy_head; temp != NULL; temp = temp->next)
	{
		if (temp->item) {
			strcat(cmd_to_ret, temp->item);
		}
	}

	for (temp = copy_head; temp != NULL; temp = temp->next)
	{
		free(temp->item);
	}

	return cmd_to_ret;

}