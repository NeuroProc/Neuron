#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXLEN 10
#define MAXNUM 100

int randome(const int);
int getmax(int [], const int, const int);

int main(void)
{
    //int max = 0;
    int a[MAXLEN];

    srand(time(NULL));
    

    for (int i = 0; i < MAXLEN; i++) {
        a[i] = randome(MAXNUM);
        printf("%d ",a[i]);
    }
    printf("\n");

    printf("%d\n",getmax(a, 0, MAXLEN));

    return 0;
}

int randome(int n)
{
    return (rand() % n);
}

int getmax(int a[], const int l, const int r)
{
    int maxl = 0, maxr = 0;

    if (r == (l - 1))
        return a[r];
    
    for (int i = l; i < (((r - l) / 2) + l); i++) {
        if (a[i] > maxl)
            maxl = a[i];
    }

    for (int i = (((r - l) / 2) + l); i < r; i++) {
        if (a[i] > maxr)
            maxr = a[i];
    }

    return ((maxl > maxr) ? getmax(a, l, (((r - l) / 2)) + l) : getmax(a, (((r - l) / 2) + l), r));
}
        
