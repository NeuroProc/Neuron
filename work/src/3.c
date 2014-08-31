#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *test(void);

int main(void)
{
    char *tmp;
    
    printf("%s\n", (tmp = test()));

    free(tmp);

    return 0;
}

char *test(void)
{
    char *res;

    res = malloc(6);
    strcpy(res, "hello");

    return res;
}
