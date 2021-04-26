#include <stdio.h>

int main(void)
{
  char c;
  printf("%zu %zu\n", sizeof(char), sizeof c);
  int i;
  printf("%zu %zu\n", sizeof(int), sizeof i);
  double d;
  printf("%zu %zu\n", sizeof(double), sizeof d);
  return 0;
}
