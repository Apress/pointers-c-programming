#include <stdio.h>

void abc(int *a, int *b, int *c)
{
  *a += *c;
  *b += *c;
}

void abc_restrict(int *a, int *b, int * restrict c)
{
  *a += *c;
  *b += *c;
}

int main(void)
{
  int x, y;
  x = y = 13;
  // No problem here. We haven't made any restrict
  // promises
  abc(&x, &y, &x);
  printf("%d %d\n", x, y);

  // We break the promise here by passing
  // using &a both as argument a and c in
  // in the function
  x = y = 13;
  abc_restrict(&x, &y, &x);
  printf("%d %d\n", x, y);

  return 0;
}
