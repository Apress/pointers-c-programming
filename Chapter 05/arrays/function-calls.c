#include <stdio.h>

void pointer(int *a)
{
  printf("pointer: %zu %zu\n", sizeof a, sizeof *a);
}

void array(int a[])
{
  printf("array: %zu %zu\n", sizeof a, sizeof *a);
}

void array_with_size(int a[50])
{
  printf("array[50]: %zu %zu\n", sizeof a, sizeof *a);
}

void array_with_parameter_size(int n, int a[n])
{
  printf("array[n]: %zu %zu\n", sizeof a, sizeof *a);
}

void size_constrained(int a[static 4])
{
  printf("size constrained a[0] == %d\n", a[0]);
}

void indirect_size_constrained(int a[static 2])
{
  size_constrained(a); // No warning, though 2 < 4
}

void pointer_to_array(int (*a)[3])
{
  printf("*a: %zu = %zu x %zu\n",
         sizeof *a, sizeof *a / sizeof **a, sizeof **a);
}

void pointer_to_array_n(int n, int (*a)[n])
{
  printf("*a with n = %d: %zu = %zu x %zu\n",
         n, sizeof *a,
         sizeof *a / sizeof **a, sizeof **a);
}

void indirect_pointer_to_array(int n, int (*array)[1])
{
  pointer_to_array(array); // Warning, ok bcause 1 < 3
}

int main(void)
{
  int n = 100;
  int a[n]; a[0] = 42;
  int b[2]; b[0] = 13;
  int *p = b;

  printf("declared: %zu %zu\n", sizeof a, sizeof *a);
  pointer(a);
  array(a);
  array_with_size(a); // Ok, 100 > 5
  array_with_size(b); // No warning although 2 < 50
  array_with_parameter_size(n, a); // Ok, a has size n
  array_with_parameter_size(n, b); // No warning but 2 < 100

  size_constrained(b); // Warning (correct, 2 < 4)
  size_constrained(p); // No warning, even though p == b

  indirect_size_constrained(b); // No warning...

  pointer_to_array(&a); // Ok, 100 > 3
  pointer_to_array(&b); // Warning (correct 2 < 3)
  pointer_to_array_n(10, &a); // Ok since 100 > 10
  pointer_to_array_n(50, &b); // No warning, although 2 < 50

  pointer_to_array(p); // Warning, ok since p does not point to array
  pointer_to_array_n(10, p); // Warning, ditto

  indirect_pointer_to_array(2, b);


  return 0;
}
