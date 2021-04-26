#include <stdio.h>

int main(void)
{
  int *p = 0;
  int const **q = 0;
  int const i = 42;

  q = (int const **)&p;
  *q = &i;

  // Now I have an int alias to an int const!
  printf("&i == %p, *p == %p\n", (void *)&i, (void *)p);

  *p = 5; // DANGER: We are trying to change const int i to 5
  // This may or may not actually change i. It is up to the C compiler
  printf("i == %d / %d\n", i, *p);

  return 0;
}
