#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "errors.h"

#define MAX_RESPONSE_SIZE 4096
#define TCP_LIMIT 2

char *client_executor(char *cmd_data);

#endif