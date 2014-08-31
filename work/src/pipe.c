#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(void) {

	int p[2], fd;
	char *buf = "123456";
	char tmpbuf[100];
    char *fifo = "/tmp/myfifo";

	pipe(p);
    mkfifo(fifo, 0666);
    
	if (fork() == 0) {
		sleep(3);

        fd = open(fifo, O_WRONLY);
        write(fd, "FIFO", 5);
        close(fd);
        
		write(p[1], "PIPE", 5);
        close(p[1]);

        exit(0);
	}

    fd = open(fifo, O_RDONLY);
    read(fd, tmpbuf, 100);
    printf("%s\n", tmpbuf);
    close(fd);
    unlink(fifo);
    
    memset(tmpbuf, 0, 100);
    
    read(p[0], tmpbuf, 100);
	printf("%s\n", tmpbuf);
    close(p[0]);
/*	
	write(p[1], "OLOLO", 6);
	read(p[0], tmpbuf, 100);
	printf("%s\n", tmpbuf);
*/
	return 0;
}
	

	
