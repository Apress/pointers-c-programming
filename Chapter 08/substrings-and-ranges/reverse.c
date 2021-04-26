#include <stdio.h>
#include "substr.h"

int main(void)
{
  printf("reverse\n");
  do {
    char x[] = "abc";
    substr_rev(as_substr(x));
    printf("%s\n", x);
  } while(0);
  do {
    char x[] = "";
    substr_rev(as_substr(x));
    printf("%s\n", x);
  } while(0);
  do {
    char x[] = "a";
    substr_rev(as_substr(x));
    printf("%s\n", x);
  } while(0);
  do {
    char x[] = "ab";
    substr_rev(as_substr(x));
    printf("%s\n", x);
  } while(0);

  printf("\n");
  return 0;
}
