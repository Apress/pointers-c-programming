#include <stdio.h>

void add_array(int n, int array[n], int x)
{
  for (int i = 0; i < n; i++) {
    array[i] += x;
  }
}

void add_pointers(int *begin, int *end, int x)
{
  for ( ; begin < end; begin++) {
    *begin += x;
  }
}

int sum_array(int n, int array[n])
{
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += array[i];
  }
  return sum;
}

int sum_pointers_(int *begin, int *end)
{
  int sum = 0;
  for ( ; begin < end; begin++) {
    sum += *begin;
  }
  return sum;
}

int sum_pointers(int *begin, int *end)
{
  int sum = 0;
  while (begin < end) {
    sum += *begin++;
  }
  return sum;
}

void swap_array(int array[], int i, int j)
{
  int tmp = array[j];
  array[j] = array[i];
  array[i] = tmp;
}

void reverse_array(int n, int array[n])
{
  for (int i = 0; i < n/2; i++) {
    swap_array(array, i, n - i - 1);
  }
}

void swap_pointers(int *i, int *j)
{
  int tmp = *i;
  *i = *j;
  *j = tmp;
}

void reverse_pointers(int *begin, int *end)
{
  if (end <= begin) return;
  end--; // point to last element
  while (begin < end) {
    swap_pointers(begin++, end--);
  }
}

void print_array(int n, int array[n])
{
  printf("[ ");
  for (int i = 0; i < n; i++)
    printf("%d ", array[i]);
  printf("]\n");
}

int main(void)
{
  int array[] = { 1, 2, 3, 4, 5 };
  int n = sizeof array / sizeof *array;
  add_array(n, array, 2);
  print_array(n, array);

  add_pointers(array, array + n, -2);
  print_array(n, array);

  printf("%d %d\n",
         sum_array(n, array),
         sum_pointers(array, array + n));

  reverse_array(n, array);
  print_array(n, array);

  reverse_pointers(array, array + n);
  print_array(n, array);

  return 0;
}
