#include <stdio.h>

int main(void)
{
  char c = 1;
  printf("%d %p\n", c, (void *)&c);
  int i = 2;
  printf("%d %p\n", i, (void *)&i);
  double d = 3.0;
  printf("%f %p\n", d, (void *)&d);
  return 0;
}
