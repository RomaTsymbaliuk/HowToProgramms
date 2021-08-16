#include "menu_processor.h"

int main(int argc, char *argv[]) {
	struct cmd_data *c = process_user_input(argc, argv);
	if (c) {
		if (run_user_cmd(c) == FALSE)
			return FALSE;
		
		return TRUE;
	}
	else {
		return FALSE;
	}
}
