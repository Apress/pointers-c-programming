#include <stdio.h>

int little_endianess(int i)
{
  char *cp = (char *)&i;
  int result = 0, coef = 1;
  for (int j = 0; j < sizeof i; j++) {
    result += coef * cp[j];
    coef *= 256;
  }
  return result;
}

int big_endianess(int i)
{
  char *cp = (char *)&i;
  int result = 0, coef = 1;
  for (int j = sizeof i - 1; j >= 0; j--) {
    result += coef * cp[j];
    coef *= 256;
  }
  return result;
}

int main(void)
{
  for (int i = 0; i < 10; i++) {
    printf("%d: little = %d, big = %d\n",
           i, little_endianess(i), big_endianess(i));
  }

  return 0;
}
