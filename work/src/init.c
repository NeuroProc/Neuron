#include <stdio.h>
#include <stdlib.h>

int lol(int **a) {
	return sizeof(a);
}

int main(void) {
	//int mas[10] = { -1 };
	int **test;
	//int lol[10000000000];
	
	if ()

	test = malloc(sizeof(int *) * 100000);
	for (int i = 0; i < 100000; i++)
		*test = malloc(sizeof(int) * 100000);

	printf("%d %d\n",lol(test),sizeof(int *));
	free(test);
	/*
	for(int i = 0; i < 100000; i++)
		for (int j = 0; j < 100000; j++)
			printf("%d ",test[i][j]);

	printf("\n");
	*/

	return 0;
}
