#include <stdio.h>

int main(void)
{
  int i = 42;
  int *ip = &i;
  const int *cp = &i;

  for (int j = 0; j < 10; ++j) {
    i++;
    printf("*ip == %d, *cp == %d\n", *ip, *cp);
  }

  return 0;
}
