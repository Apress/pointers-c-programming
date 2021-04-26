#include <stdio.h>
#include <string.h>

int int_compare(void const *x, void const *y)
{
  // Get the objects, and interpret them as integers
  int const *a = x;
  int const *b = y;
  return *a - *b;
}

int string_compare(void const *x, void const *y)
{
  // Get the objects and interpet them as strings
  char * const *a = x;
  char * const *b = y;
  return strcmp(*a, *b);
}

int main(void)
{
  int int_array[] = { 10, 5, 30, 15, 20, 30 };
  int int_array_length =
    sizeof int_array / sizeof *int_array;

  qsort(int_array, int_array_length,
        sizeof *int_array, int_compare);
  printf("int_array = ");
  for (int i = 0; i < int_array_length; i++) {
    printf("%d, ", int_array[i]);
  }
  printf("\n");

  char *string_array[] = { "foo", "bar", "baz" };
  int string_array_length =
    sizeof string_array / sizeof *string_array;

  qsort(string_array, string_array_length,
        sizeof *string_array, string_compare);
  printf("string_array = ");
  for (int i = 0; i < string_array_length; i++) {
    printf("%s, ", string_array[i]);
  }
  printf("\n");

  return 0;
}
