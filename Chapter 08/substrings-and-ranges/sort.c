#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "substr.h"

int cmp_func(void const *x, void const *y)
{
  substr const *a = x;
  substr const *b = y;
  return substr_cmp(*a, *b);
}

int main(void)
{
  char const *x = "foobarbaz";
  int n = strlen(x);

  substr suffixes[n];
  for (int i = 0; i < n; ++i) {
    suffixes[i] = slice((char *)x, i, n);
  }

  qsort(suffixes, n, sizeof *suffixes, cmp_func);

  for (int i = 0; i < n; ++i) {
    print_substr(suffixes[i]); putchar('\n');
  }

  return 0;
}
