#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char *argv[]){
	int opt;
	
	while ((opt = getopt(argc, argv, "abcd")) != -1) {
		switch (opt) {
		case 'a':
		       	printf("a chosen\n");
			break;			  
		case 'b':
			printf("b chosen\n");
			break;
		case 'c':
			printf("c choosen\n");
			break;
		default:
			printf("Unknown option\n");
		}
	}

	return 0;
}
