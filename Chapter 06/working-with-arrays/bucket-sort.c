#include <stdio.h>

void compute_buckets(int n, char *array[n], int buckets[256])
{
  for (int i = 0; i < 256; i++) {
    buckets[i] = 0;
  }

  for (int i = 0; i < n; i++) {
    unsigned char bucket = (unsigned char)array[i][0];
    buckets[bucket]++;
  }

  int m = n;
  for (int i = 256 - 1; i >= 0; i--) {
    int count = buckets[i];
    buckets[i] = m - count;
    m -= count;
  }
}

void sort_strings(int n, char *input[n], char *output[n])
{
  int buckets[256];
  compute_buckets(n, input, buckets);
  for (int i = 0; i < n; i++) {
    unsigned char bucket = (unsigned char)input[i][0];
    int index = buckets[bucket]++;
    output[index] = input[i];
  }
}

int main(void)
{
  char *array[] = {
    "foo", "boo", "bar", "qoo", "qar", "baz", "qux", "qaz"
  };
  int n = sizeof array / sizeof *array;
  char *output[n];

  sort_strings(n, array, output);

  for (int i = 0; i < n; i++) {
    printf("%s\n", output[i]);
  }

  return 0;
}
