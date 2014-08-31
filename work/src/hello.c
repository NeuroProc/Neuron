/*
 * NOMER
 */


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{ 
  
  if (argc != 3)
    {
      printf("too few arguments\n");
      exit(1);
    }

  printf("you variant: %d\n",(atoi(argv[1]) % atoi(argv[2])) + 1);

  return 0;
}
