#include <unistd.h>
#include <stdio.h>
#include <signal.h>
struct sigaction act;

void handler(int sig) {
   printf("oops\n");
}

int main( int argc, char **argv ) {
   printf("%d\n", getpid());
   act.sa_handler = handler;
   sigaction(SIGUSR1, &act, NULL);
   pause();
   return 0;
}
