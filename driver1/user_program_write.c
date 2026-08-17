#include <stdio.h>
#include <unistd.h>


int main(void){

	FILE* fp = fopen("/proc/nara-proc", "w");
	char buffer[200];
	printf("Enter anything u want to enter \n");
	scanf("%s",buffer);
	fprintf(fp, "%s",buffer);	
	return 0;
}

