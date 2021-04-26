#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
bool is_sorted(void const *array,
               size_t len, size_t obj_size,
               compare_function cmp)
{
  for (int i = 1; i < len; i++) {
    void const *a = (char *)array + (i - 1) * obj_size;
    void const *b = (char *)array + i * obj_size;
    if (cmp(a, b) > 0) {
      // a is larger than b, so the array is not sorted
      return false;
    }
  }
  return true;
}

int main(void)
{
  int int_array[] = { 10, 5, 30, 15, 20, 30 };
  int int_array_length =
    sizeof int_array / sizeof *int_array;

  if (is_sorted(int_array, int_array_length,
                sizeof *int_array, int_compare)) {
    printf("int_array is sorted\n");
  } else {
    printf("int_array is not sorted\n");
  }
  qsort(int_array, int_array_length,
        sizeof *int_array, int_compare);
  if (is_sorted(int_array, int_array_length,
                sizeof *int_array, int_compare)) {
    printf("int_array is sorted\n");
  } else {
    printf("int_array is not sorted\n");
  }

  char *string_array[] = { "foo", "bar", "baz" };
  int string_array_length =
    sizeof string_array / sizeof *string_array;

  if (is_sorted(string_array, string_array_length,
                sizeof *string_array, string_compare)) {
    printf("string_array is sorted\n");
  } else {
    printf("string_array is not sorted\n");
  }
  qsort(string_array, string_array_length,
        sizeof *string_array, string_compare);
  if (is_sorted(string_array, string_array_length,
                sizeof *string_array, string_compare)) {
    printf("string_array is sorted\n");
  } else {
    printf("string_array is not sorted\n");
  }

  return 0;
}
