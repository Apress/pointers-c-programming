#include <stdio.h>
#include "substr.h"

int main(void)
{
  substr x = as_substr("foobarbazqux");
  substr y = as_substr("ba");
  substr_iter iter = x;

  printf("searching for %s in %s\n", y.begin, x.begin);
  for (substr occ = next_occurrence(&iter, y, 0);
       !null_substr(occ);
       occ = next_occurrence(&iter, y, 0)) {
    printf("Found an occurrence at index %d\n",
           (int)(occ.begin - x.begin));
  }

  x = as_substr("xaxaxaxaxaxa");
  y = as_substr("xaxa");
  iter = x;
  printf("searching for %s in %s\n", y.begin, x.begin);
  for (substr occ = next_occurrence(&iter, y, 0);
       !null_substr(occ);
       occ = next_occurrence(&iter, y, 0)) {
    printf("Found an occurrence at index %d\n",
           (int)(occ.begin - x.begin));
  }

  iter = x;
  printf("searching for %s in %s\n", y.begin, x.begin);
  for (substr occ = next_occurrence(&iter, y, 1);
       !null_substr(occ);
       occ = next_occurrence(&iter, y, 1)) {
    printf("Found an occurrence at index %d\n",
           (int)(occ.begin - x.begin));
  }

  return 0;
}
