#include <assert.h>

void array_full_size(int A[10][10])
{
  // A becomes a pointer to length 10 arrays
  assert(sizeof A == sizeof(int (*)[10]));
  assert(sizeof *A == 10 * sizeof(int));
}

void array_incomplete_size(int A[][10])
{
  // A becomes a pointer to length 10 arrays
  assert(sizeof A == sizeof(int (*)[10]));
  assert(sizeof *A == 10 * sizeof(int));
}

void pointer(int (*A)[10])
{
  // A is explicitly a pointer to length 10 arrays
  assert(sizeof A == sizeof(int (*)[10]));
  assert(sizeof *A == 10 * sizeof(int));
}

int main(void)
{
  int A[10][10];
  assert(sizeof A == 10 * 10 * sizeof(int));
  array_full_size(A);
  array_incomplete_size(A);
  pointer(A);

  int B[5][10];
  assert(sizeof B == 5 * 10 * sizeof(int));
  // B's first dimension is wrong, but no warnings
  array_full_size(B);
  array_incomplete_size(B);
  pointer(B);

  int C[10][5];
  assert(sizeof C == 10 * 5 * sizeof(int));
  // You get warnings here, because the
  // second dimension doesn't match
  array_full_size(C);
  array_incomplete_size(C);
  pointer(C);

  return 0;
}
