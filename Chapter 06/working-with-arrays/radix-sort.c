#include <stdio.h>
#include <assert.h>

void bucket_sort(int n, int offset,
                 int const input[n], int output[n])
{
  int buckets[256];
  for (int i = 0; i < 256; i++) {
    buckets[i] = 0;
  }

  for (int i = 0; i < n; i++) {
    unsigned char bucket = (input[i] >> 8 * offset) & 0xff;
    buckets[bucket]++;
  }

  int m = n;
  for (int i = 256 - 1; i >= 0; i--) {
    int count = buckets[i];
    buckets[i] = m - count;
    m -= count;
  }

  for (int i = 0; i < n; i++) {
    unsigned char bucket = (input[i] >> 8 * offset) & 0xff;
    int index = buckets[bucket]++;
    output[index] = input[i];
  }
}

void radix_sort(int n, int array[n])
{
  // It is *very* unlikely that sizeof an integer is odd, but if
  // it is, you need to move the results from helper
  // to array. I assume that we have an even number of bytes
  // because that is practically always true for int
  static_assert(sizeof *array % 2 == 0,
                "integer sizes must be powers of two");

  // Helper buffer; handle input/output switches
  // when bucket sorting
  int helper[n];
  // For switching between the buffers
  int *buffers[] = { array, helper };
  int bucket_input = 0;

  for (int offset = 0; offset < sizeof *array; offset++) {
    bucket_sort(n, offset,
                buffers[bucket_input],
                buffers[!bucket_input]);
    bucket_input = !bucket_input;
  }

}

// Both left and right must point to legal addresses
int *scan_right(int *left, int *right)
{
  while (left < right) {
    if (*left >= 0) break;
    left++;
  }
  return left;
}

// Both left and right must point to legal addresses
int *scan_left(int *left, int *right)
{
  while (left < right) {
    if (*right < 0) break;
    right--;
  }
  return right;
}

// Both left and right must point to legal addresses
void swap(int *left, int *right)
{
  int i = *left;
  *left = *right;
  *right = i;
}

int split(int n, int array[n])
{
  int *left = array, *right = array + n - 1;
  while (left < right) {
    left = scan_right(left, right);
    right = scan_left(left, right);
    swap(left, right);
  }
  return left - array;
}

void reverse(int n, int array[n])
{
  int *left = array, *right = array + n - 1;
  while (left < right) {
    swap(left++, right--);
  }
}

void sort_int(int n, int array[n])
{
  if (n <= 0) return;
  int m = split(n, array);
  if (m > 0) {
    radix_sort(m, array);
    reverse(m, array);
  }
  if (m < n) {
    radix_sort(n - m, array + m);
  }
}

int main(void)
{
  int array[] = { -1, -2, 13, 12, 4, 4200, 13, 6, 14, -3, 42, 13 };
  int n = sizeof array / sizeof *array;

  radix_sort(n, array);
  for (int i = 0; i < n; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");

  sort_int(n, array);
  for (int i = 0; i < n; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");

  return 0;
}
