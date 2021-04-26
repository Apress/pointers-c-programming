#include <stdio.h>
#include <stdalign.h>

int f(int *i, long *l)
{
  *i = -1;
  *l = 0;
  return *i;
}

int g(char *c, long *l)
{
  *c = -1;
  *l = 0;
  return *c;
}

int h(double *c, long *l)
{
  *c = -1;
  *l = 0;
  return (int)*c;
}

int main(void)
{
  long x;

  int i = f((int *)&x, &x);
  printf("x == %ld, f(&x,&x) == %d\n", x, i);

  i = g((char *)&x, &x);
  printf("x == %ld, g(&x,&x) == %d\n", x, i);

  i = h((double *)&x, &x);
  printf("x == %ld, h(&x,&x) == %d\n", x, i);

  return 0;
}
