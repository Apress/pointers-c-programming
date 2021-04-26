#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  int *ip = calloc(10, sizeof *ip);
  double *dp = calloc(20, sizeof *dp);

  free(ip);
  free(dp);

  printf("%zu %zu\n", SIZE_MAX, 10 * (SIZE_MAX / 1));
  char *x = calloc(10, SIZE_MAX / 5);
  if (x) {
    printf("success???\n");
  } else {
    perror("Allocation error");
  }

  return 0;
}
