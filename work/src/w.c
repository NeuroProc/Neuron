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

	int a = 5 == 1 ? 2 : 3;

	printf("%d\n", a);

	return 0;
}
		
