#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "executor.h"

char *client_executor(char *cmd_data)
{
	FILE *fp;
	char result[1024];
	long int cmd_res_size = 0;
	char *cmd_to_ret;
	int i = 1;

	fp = popen(cmd_data, "r");
	if (fp == NULL) {
		printf("Failed to run command!\n");
		return NULL;
	}

	cmd_to_ret = malloc(1024);
	if (!cmd_to_ret) {
		printf("Memory problem\n");
		return NULL;
	}
	while (fgets(result, sizeof(result), fp) != NULL) {
		if (strlen(cmd_to_ret) > 1000) {
			i++;
			cmd_to_ret = realloc(cmd_to_ret, i * 1024);
		}
		strcat(cmd_to_ret, result);
	}

//	printf("cmd_to_ret i: %d\n sizeof cmd_to_ret : %d", i, sizeof(cmd_to_ret));
//	printf("%s", cmd_to_ret);
	pclose(fp);

	return cmd_to_ret;
}