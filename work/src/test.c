#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//char *getstring(void);
//int open(FILE **f);

int main(void)
{
  char *str = NULL;
  //FILE *test;


  printf("%p\n",str);
  str = malloc(2);
  printf("%p\n",str);
  scanf("%s",str);
  printf("%s : %p\n",str,str);

  printf("enter str:\n");

  //str = getstring();
  /*
  str = (char *)malloc(5);
  str[0] = '0';
  str[1] = '1';
  str[2] = '2';
  str[3] = '3';
  str[4] = '4';
  str[5] = '5';
  str[6] = '6';
  str[7] = '7';
  */
  //printf("%s\n",str);
  //if (!open(&test))
  //    fprintf(test,"test\n");
  
  return 0;
}
/*
int open(FILE **f)
{
  if ((*f = fopen("pidor.txt","w")) == NULL)
    return 1;
  else
    return 0;
}
*/
/*
char *getstring(void)
{
  char *tmp, c;
  int i = 0;

  while ((c = getchar()) != '\n')
    {
      //tmp[i] = (char *)malloc(sizeof(char));
      tmp[i] = c;
      printf("%d: %c %d %d\n",i,tmp[i],tmp,&tmp[i]);
      i++;
    }
  tmp[i] = '\0';

  return tmp;
}
*/
