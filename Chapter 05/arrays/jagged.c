#include <stdio.h>
#include <assert.h>

int main(void)
{
  double *A[] = {
    (double[]){1},
    (double[]){2, 3},
    (double[]){4, 5, 6}
  };

  int n = sizeof A / sizeof *A;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= i; j++) {
      printf("%2.2f ", A[i][j]);
    }
    printf("\n");
  }

  // not true: assert(sizeof A == 6 * sizeof(double));
  assert(sizeof A == 3 * sizeof(double *));
  assert(sizeof A[0] == sizeof(double *));
  assert(sizeof A[1] == sizeof(double *));
  assert(sizeof A[2] == sizeof(double *));

  double row0[] = {1};
  double row1[] = {2, 3};
  double row2[] = {4, 5, 6};
  double *B[] = { row0, row1, row2 };

  assert(sizeof B == sizeof A);
  assert(sizeof B[0] == sizeof A[0]);

  double **p_A = A;
  assert(p_A[0] == A[0]);
  assert(p_A[1] == A[1]);
  assert(p_A[0][0] == A[0][0]);

  double *p_A1 = A[1];
  assert(p_A1[0] == A[1][0]);
  assert(p_A1[1] == A[1][1]);

  return 0;
}
