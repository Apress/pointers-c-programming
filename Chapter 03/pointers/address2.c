#include <stdio.h>

int main(void)
{
  int i = 1;
  int *pi = &i;
  int **ppi = &pi;

  printf("i = %d, &i = %p\n", i, &i);
  printf("pi = %p, &pi = %p\n", pi, &pi);
  printf("ppi = %p, &ppi = %p\n", ppi, &ppi);
  printf("----------------\n");

  printf("i = %d, pi = %p\n", *pi, *ppi);
  printf("i = %d\n", **ppi);
  printf("----------------\n");

  *pi = 2;
  printf("i = %d, pi = %p, ppi = %p\n", i, pi, ppi);
  **ppi = 3;
  printf("i = %d, pi = %p, ppi = %p\n", i, pi, ppi);
  printf("----------------\n");

  int i2 = 42;
  pi = &i2;
  printf("i2 = %d, *pi = %d\n", i2, *pi);


  return 0;
}
