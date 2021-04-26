#include <stdio.h>
#include <string.h>

int int_compare(void const *x, void const *y)
{
  // Get the objects, and interpret them as integers
  int const *a = x;
  int const *b = y;
  return *a - *b;
}

int string_compare(const void *x, const void *y)
{
  // Get the objects and interpet them as strings
  char * const *a = x;
  char * const *b = y;
  return strcmp(*a, *b);
}

typedef int (*compare_function)(void const *, void const *);

void swap(void *a, void *b, size_t obj_size)
{
  char tmp[obj_size];
  memcpy(&tmp, a, obj_size);
  memcpy(a, b, obj_size);
  memcpy(b, &tmp, obj_size);
}

void swap_down(char *start, char *current,
               size_t obj_size,
               compare_function cmp)
{

  while (current != start) {
    char *prev = current - obj_size;
    if (cmp(prev, current) <= 0) break; // done swapping
    swap(prev, current, obj_size);
    current = prev;
  }
}

void insertion_sort(void *array,
                    size_t len, size_t obj_size,
                    compare_function cmp)
{
  char *start = array;
  for (int i = 1; i < len; i++) {
    swap_down(start, start + i * obj_size, obj_size, cmp);
  }
}

int main(void)
{
  int int_array[] = { 10, 5, 30, 15, 20, 30 };
  int int_array_length =
    sizeof int_array / sizeof *int_array;

  insertion_sort(int_array, int_array_length,
        sizeof *int_array, int_compare);
  for (int i = 0; i < int_array_length; i++) {
    printf("%d ", int_array[i]);
  }
  printf("\n");

  char *string_array[] = { "foo", "bar", "baz" };
  int string_array_length =
    sizeof string_array / sizeof *string_array;

  insertion_sort(string_array, string_array_length,
        sizeof *string_array, string_compare);
  for (int i = 0; i < string_array_length; i++) {
    printf("%s ", string_array[i]);
  }
  printf("\n");

  return 0;
}
