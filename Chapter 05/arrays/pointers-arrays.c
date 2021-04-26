#include <stdio.h>
#include <assert.h>

void not_what_you_want(int array[])
{
  // sizeof(array) is sizeof(int *) here!
  printf("%zu\n", sizeof array);
  // Here, the array and the address of the array
  // are different. array is a local variable
  // that holds a pointer to the array!
  printf("%p %p\n", (void *)array, (void *)&array);
}

int main(void)
{
  int array[] = { 1, 2, 3, 4, 5 };
  int *ap = array;
  int (*ap2)[] = &array;

  printf("sizeof array == %zu, sizeof ap == %zu, sizeof ap2 == %zu, sizeof *ap2\n",
         sizeof array, sizeof ap, sizeof ap2);
  printf("%p %p %p %p %p %p\n", (void *)array, (void *)&array,
         (void *)ap, (void *)&ap, (void *)ap2, (void *)&ap2);

  int n = sizeof array / sizeof *array;
  for (int i = 0; i < n; i++) {
    assert(array[i] == ap[i]);
    assert(array + i == ap + i);
    assert(*(array + i) == *(ap + i));
  }

  return 0;
}
