#include <stdio.h>

void sort_chars(int n, unsigned char array[n])
{
  int buckets[256];
  for (int i = 0; i < 256; i++) {
    buckets[i] = 0;
  }

  for (int i = 0; i < n; i++) {
    unsigned int bucket = array[i];
    buckets[bucket]++;
  }

  int k = 0;
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < buckets[i]; j++) {
      array[k++] = (unsigned char)i;
    }
  }
}

int main(void)
{
  unsigned char array[] = { 'f', 'o', 'o', 'b', 'a', 'r' };
  int n = sizeof array / sizeof *array;

  sort_chars(n, array);
  for (int i = 0; i < n; i++) {
    printf("%c", array[i]);
  }
  printf("\n");

  return 0;
}
