#include <stdio.h>

int main(void)
{
  int C[2][2][3] = {
    { { 1, 2, 3 }, {  4,  5,  6 } },
    { { 7, 8, 9 }, { 10, 11, 12 } }
  };

  int dim1 = sizeof C / sizeof C[0];
  int dim2 = sizeof C[0] / sizeof C[0][0];
  int dim3 = sizeof C[0][0] / sizeof C[0][0][0];

  printf("C dimensions %d x %d x %d\n", dim1, dim2, dim3);

  printf("First element in each row: ");
  int (*first_dim_p)[2][3] = C;
  int (*first_end)[2][3] = C + dim1;
  for ( ; first_dim_p < first_end; first_dim_p++) {
    printf("%d ", *(int*)first_dim_p);
  }
  printf("\n");

  printf("First element in each column: ");
  int (*second_dim_p)[3] = (int (*)[3])C;
  int (*second_end)[3] = (int (*)[3])C + dim1 * dim2;
  for ( ; second_dim_p < second_end; second_dim_p++) {
    printf("%d ", *(int*)second_dim_p);
  }
  printf("\n");

  return 0;
}
