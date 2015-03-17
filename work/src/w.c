#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(void) {
/*	
	pid_t pid = 0;

	if (!fork()) {
		printf("OLOLO\n");
		sleep(5);
	}
	else {
		printf("%ld %ld %d %d\n",waitpid(-1,NULL,0),waitpid(-1,NULL,0),errno, ECHILD);
	}
*/
	
	unsigned long a = 0;
	

	printf("%lu\n", ~a);

	return 0;
}
		
