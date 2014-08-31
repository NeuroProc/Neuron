#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 80

int main(int argc, char *argv[]) {

	char mas[BUFLEN], tmp[BUFLEN];
	int a, b;


	strcpy(mas, argv[1]);
	printf("%s\n", mas);

	a = atoi(argv[2]);
	b = atoi(argv[3]);

	strcpy(tmp, mas);
	memset(mas, 0, BUFLEN);

	tmp[a] = '\0';
	sprintf(mas, "%s%s", tmp, tmp+b);
	printf("%s\n", mas);

	return 0;
}


	
