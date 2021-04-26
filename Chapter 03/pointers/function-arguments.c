#include <stdio.h>

void doesnt_mutate(int i)
{
  i += 42;
}

void mutates(int *i)
{
  *i += 42;
}

void foo(int *ip)
{
  ip++;
}

void bar(int **ip)
{
  (*ip)++;
}

int main(void)
{
  int i = 0;
  doesnt_mutate(i);
  printf("i is %d\n", i);
  mutates(&i);
  printf("i is %d\n", i);

  int *ip = 0;
  foo(ip);
  printf("%p\n", ip);
  bar(&ip);
  printf("%p\n", ip);

  return 0;
}
