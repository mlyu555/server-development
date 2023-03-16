#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	printf("sizeof(char) = %d\n", sizeof(char));
	printf("sizeof(long) = %d\n", sizeof(long));
	printf("sizeof(int)  = %d\n", sizeof(int));
	printf("sizeof(short)= %d\n", sizeof(short));
	printf("sizeof(char*)= %d\n", sizeof(char*));
	printf("sizeof(void*)= %d\n", sizeof(void*));
	return 0;
}
