#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

int main(int argc, char **argv)
{
    char *s = NULL;

    printf("%d\n",PATH_MAX);
    //PRINTF("%LU\N",SIZEOF(S));

    //S = MALLOC(10);
    //S = "LOL";

    //ARGV[1] = REALLOC(ARGV[1],STRLEN(argv[1]) + strlen(s));
    //sprintf(argv[1],"%s%s", argv[1],s);

    //printf("%lu\n",strlen(argv[1]));
    //printf("%s %s\n",argv[1],argv[2]);
    //free(s);

    return 0;
}
