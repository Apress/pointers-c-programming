#include <stdlib.h>
#include <stdio.h>

typedef int T;

int main(void)
{
  int *ip = malloc(sizeof(int));
  double *dp = malloc(sizeof(double));

  struct S { int i; double d; };
  struct S *sp = malloc(sizeof(struct S));

  T *p = malloc(sizeof *p);

  free(ip);
  free(dp);
  free(sp);
  free(p);

  int n = 10;
  int *arr = malloc(n * sizeof *arr);

  free(arr);

  char *x = malloc(SIZE_MAX);
  if (x) {
    printf("success???\n");
  } else {
    perror("Allocation error");
    printf("couldn't allocate %zu bytes.\n", SIZE_MAX);
  }

  return 0;
}
