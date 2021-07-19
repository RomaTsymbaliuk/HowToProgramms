#include <stdio.h>
#include <stdarg.h>

int sum_counter(int argc, ...){	
	va_list ap;
	va_start(ap, argc);
	int sum = 0;
	for (int i = 1; i<=argc; i++){
		int a = va_arg(ap, int);
		sum+=a;
	}
	return sum;
}
int main(int argc, char* argv[]){
	printf("\n%d", sum_counter(5, 1, -2, 3, 8, 87));
	return 0;
}
