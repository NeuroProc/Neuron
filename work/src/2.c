#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct tm t;
	int i = 0;

	//sscanf(argv[1], "%d/%d/%d", &(t.tm_mday), &(t.tm_mon), &(t.tm_mday));
	//scanf("%d/%d/%d 1", &(t.tm_mday), &(t.tm_mon), &(t.tm_mday));
	t.tm_mday = atoi(strtok(argv[1],".")); 
	t.tm_mon = atoi(strtok(NULL,"."));
	t.tm_year = atoi(strtok(NULL,"."));
	//memset(&t, 0, sizeof(t));
	//t.tm_mday = 1;
	//t.tm_mon = 1;
	//t.tm_year = 1970;

	printf("%d.%d.%d\n", t.tm_mday, t.tm_mon, t.tm_year);

	printf("%lu    %lu\n",mktime(&t),time(NULL));
	printf("%d\n", t.tm_wday);

	//sscanf("1511111", "%d", &i);
	//printf("%d\n", i);

	//itoa(i);

	return 0;
}
