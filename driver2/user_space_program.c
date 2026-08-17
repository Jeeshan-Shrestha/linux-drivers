#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){

	int fd = open("/dev/mydevice",O_RDONLY);
	if (fd < 0){
		printf("Error opening the file");
		return -1;
	}
	printf("Successfully opened the file\n");
	close(fd);
	return 0;
}

