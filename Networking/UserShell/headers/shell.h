#ifndef SHELL_H
#define SHELL_H

#define SHELL_INIT "**********************************************\n"   \
					"*********************************************\n"   \
					"***************   USER SHELL   **************\n"   \
					"*********************************************\n"   \
					"*********************************************\n"   \
					"***\t-Think twice before using it-\t***\n"    \
					"*********************************************\n"   \
					"*********************************************\n"   \

int shell_init();
void shell_loop();
void shell_help();
int shell_parse_input();
int shell_exec_input(); 


#endif /* SHEELL_H */
