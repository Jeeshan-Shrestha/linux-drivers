#include <stdio.h>
#include <unistd.h>

int main(void){

	FILE* fp = fopen("/proc/nara-proc", "r");
	if (fp == NULL){
		printf("Error Openinig file\n");
		return -1;
	}
	int c;
	while ((c = fgetc(fp)) != EOF){
		printf("%c",c);
		fflush(stdout); //throw what ever is in the stdout immediately
					// without using the buffer
		sleep(10);
	}

	return 0;
}
