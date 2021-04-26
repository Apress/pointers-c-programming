#include <stdio.h>

int main(int argc, char **argv)
{
  void *vp = 0;
  char *cp = 0;
  int  *ip = 0;
  long *lp = 0;

  for (int i = 0; i < 5; i++) {
    char *p = vp; // so we can do arithmetic
    printf("char: %p %p ",  cp + i, p + i * sizeof(char));
    printf("int: %p %p ",   ip + i, p + i * sizeof(int));
    printf("long: %p %p\n", lp + i, p + i * sizeof(long));
  }

  return 0;
}
