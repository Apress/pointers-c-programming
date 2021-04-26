#include <stdlib.h>
#include <stdio.h>
#include <stdalign.h>

int main(void)
{
  int *ip = aligned_alloc(alignof(int), sizeof(*ip));
  double *dp = aligned_alloc(alignof(double), sizeof(*dp));

  // use pointers...

  free(ip);
  free(dp);

  return 0;
}
