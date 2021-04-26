#include <stdio.h>
#include <string.h>

void reverse(void *array, int n, int size)
{
  if (n <= 0) return; // avoid right underflow
  char *left = array;
  char *right = left + size * (n - 1);
  char tmp[size];

  while (left < right) {
    memcpy(&tmp, left, size);
    memcpy(left, right, size);
    memcpy(right, &tmp, size);
    left += size; right -= size;
  }
}

int main(void)
{
  int int_array[] = { 1, 2, 3, 4, 5 };
  int n = sizeof int_array / sizeof *int_array;
  reverse(int_array, n, sizeof *int_array);
  for (int i = 0; i < n; i++) {
    printf("%d ", int_array[i]);
  }
  printf("\n");

  char char_array[] = { 'f', 'o', 'o', 'b', 'a', 'r' };
  int m = sizeof char_array / sizeof *char_array;
  reverse(char_array, m, sizeof *char_array);
  for (int i = 0; i < m; i++) {
    printf("%c ", char_array[i]);
  }
  printf("\n");

  return 0;
}
