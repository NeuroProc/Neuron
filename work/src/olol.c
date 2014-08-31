#include <unistd.h>
#include <stdio.h>

char *s = "/home/Neuron/work/src/lab3.1.cc";

int main(void) {
	printf("%d\n",access(s,F_OK));
	return 0;
}
