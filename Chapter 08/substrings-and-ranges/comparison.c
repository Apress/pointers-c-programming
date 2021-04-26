#include <stdio.h>
#include "substr.h"

int main(void)
{
  printf("comparisons\n");
  printf("aa vs a: %d\n", substr_cmp(as_substr("aa"), as_substr("a")));
  printf("a vs aa: %d\n", substr_cmp(as_substr("a"), as_substr("aa")));
  printf("aa vs ab: %d\n", substr_cmp(as_substr("aa"), as_substr("ab")));
  printf("aa vs aa: %d\n", substr_cmp(as_substr("aa"), as_substr("aa")));
  printf("\n");
  return 0;
}
