#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
//#include <errno.h>

#define BUFLEN 10 //in bytes

int main(int argc, char *argv[])
{
	char buf[BUFLEN];
	int fin, fout, count;

	if (argc != 3)
	{
		printf("TO FEW ARGUMENTS\nUSAGE: mycopy inputfile outputfile\n");
		exit(1);
	}

	if ((fin = open(argv[1], O_RDONLY)) == -1)
	{	
		printf("!!!ERROR: OPEN() FIN\n");
		exit(2);
	}
	if ((fout = open(argv[2], O_WRONLY | O_CREAT)) == -1)
	{
		printf("!!!ERROR: OPEN() FOUT\n");
		exit(3);
	}	

	//memset(buf, 0, BUFLEN);
	while ((count = read(fin, buf, BUFLEN)) > 0)
	{
		write(fout, buf, count);
		//memset(buf, 0, BUFLEN);
	}
		
	close(fin);
	close(fout);

	printf("OK!\n");

	return 0;
}
