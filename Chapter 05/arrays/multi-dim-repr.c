#include <assert.h>

int main(void)
{
  int A[2][3] = {
    { 1, 2, 3 },
    { 4, 5, 6 }
  };

  assert(sizeof A == 2 * 3 * sizeof(int));
  assert(sizeof *A == 3 * sizeof(int));
  assert(sizeof A[0] == 3 * sizeof(int));
  assert(sizeof A[0][0] == sizeof(int));

  int *p = (int *)A;
  for (int i = 0; i < 2; i++) {
    // p now points to the first element in row i
    assert(p == A[i]);
    for (int j = 0; j < 3; j++) {
      // p points to column j in row i
      assert(A[i] + j == p);
      assert(&A[i][j] == p);
      assert(A[i][j] == *p);
      p++;
    }
  }

  int B[2][2][3] = {
    { { 1, 2, 3 }, {  4,  5,  6 } },
    { { 7, 8, 9 }, { 10, 11, 12 } }
  };

  assert(sizeof B == 2 * 2 * 3 * sizeof(int));
  assert(sizeof B[0] == 2 * 3 * sizeof(int));
  assert(sizeof B[0][0] == 3 * sizeof(int));
  assert(sizeof B[0][0][0] == sizeof(int));

  p = (int *)B;
  for (int i = 0; i < 2; i++) {
    // p now points to row i
    assert(p == (int *)B[i]);
    for (int j = 0; j < 2; j++) {
      // p now points to column j in row i
      assert(p == (int *)(B[i] + j));
      for (int k = 0; k < 3; k++) {
        // p now points to the k'th element in B[i][j]
        assert(B[i][j] + k == p);
        assert(&B[i][j][k] == p);
        assert(B[i][j][k] == *p);
        p++;
      }
    }
  }

  return 0;
}
