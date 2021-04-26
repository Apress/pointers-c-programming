#include <stdio.h>

int *bin_search(int *left, int *right, int x)
{
  while (left < right) {
    int *mid = (right - left) / 2 + left;
    if (*mid == x) return mid;
    if (*mid < x) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }
  return 0;
}

int main(void)
{
  int a[] = { 1, 2, 4 };
  int n = sizeof a / sizeof *a;

  for (int i = 0; i < 6; i++) {
    int *res = bin_search(a, a + n, i);
    printf("bin_search(%d) == %p, %d\n", i,
           (void *)res, res ? *res : -0);
  }
}
