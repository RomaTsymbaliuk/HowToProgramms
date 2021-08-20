#ifndef SHELL_H
#define SHELL_H

#define MAX_SHELL_CMD 100
#define MAX_CMD_LENGTH 200
#define MAX_ARGUMENTS_NUMBER 50
#define SHELL_INIT "**********************************************\n"   \
					"*********************************************\n"   \
					"***************   USER SHELL   **************\n"   \
					"*********************************************\n"   \
					"*********************************************\n"   \
					"***\t-Think twice before using it-\t***\n"    \
					"*********************************************\n"   \
					"*********************************************\n"   \

struct cmd {
	char *command;
	char **args;
};

int shell_init();
void shell_loop();
void shell_help();
struct cmd *shell_parse_input();
int shell_exec_input(); 


#endif /* SHEELL_H */
