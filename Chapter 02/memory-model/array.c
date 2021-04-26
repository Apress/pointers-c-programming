#include <stdio.h>

int main(void)
{
  int array[5];
  printf(" array    == %p\n", (void *)array);
  for (int i = 0; i < 5; i++) {
    printf("&array[%d] == %p\n", i, (void *)&array[i]);
  }
  printf("sizeof(array) == %zu\n", sizeof array);
  printf("5 * sizeof(int) == %zu\n", 5 * sizeof(int));

  return 0;
}
