#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
	char buf[40];

	int fd = open("/tmp/temp_f_lab4", O_RDWR);

	read(fd, buf, 9);
	buf[9] = '\0';
	printf("%s\n",buf);
	printf("%d\n",strcmp(buf,"aaaaaaaaa"));
	/*
	lseek(fd, 1, SEEK_SET);
	write(fd, "a", 1);

	memset(buf, 0, 40);
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, 40);
	printf("%s\n", buf);
	*/
	close(fd);

	return 0;
}
