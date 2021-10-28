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
	char result_cmd[MAX_RESPONSE_SIZE];
	char *response_cmd;
	char buff[30] = {0};
	int k = 0;
	int size = 0;

	fp = popen(cmd_data, "r");
	if (fp == NULL) {
		printf("Failed to run command!\n");
		return "Error";
	}

	while (fgets(buff, sizeof(buff), fp) != NULL) {

		size = size + strlen(buff) * sizeof(char);
		if (size > MAX_RESPONSE_SIZE) {
			printf("To big command result\n");
			return NULL;
		}

		if (k == 0) {
			strcpy(result_cmd, buff);
		} else {
			strcat(result_cmd, buff);
		}
		k++;
	}

	response_cmd = malloc(size);
	if (!response_cmd) {
		return NULL;
	}
	memcpy(response_cmd, result_cmd, size);

	if (WEXITSTATUS(pclose(fp)) != 0) {
		printf("\nCOMMAND ERROR!\n");
		return NULL;
	}

	return response_cmd;

}
