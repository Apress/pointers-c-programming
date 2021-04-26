#include <stdio.h>
#include <assert.h>

int main(void)
{
  int array[] = { 0, 1, 2, 3, 4 };
  int n = sizeof array / sizeof *array;
  int *jp = array;

  for (int i = 0; i < 5; i++) {
    assert(array + i == jp + i);
    assert(array[i] == jp[i]);
    assert(array[i] == *(array + i));
    assert(jp[i] == *(jp + i));

    assert(i[array] == i[jp]);
  }

  int *ip = array;
  char *p = (char *)array;
  for (int i = 0; i < n; i++) {
    printf("%p %p %p\n",
           // int array has the right offset
           (void *)(array + i),
           // int * has the right offset
           (void *)(ip + i),
           // void * jumps in bytes...
           (void *)(p + i * sizeof(int)));
  }

  char *end = (char *)array + sizeof array;
  for (ip = array, p = (char *)array;
       p != end;
       ip++, p += sizeof *ip) {
    printf("%p %p\n", (void *)ip, (void *)p);
  }

  return 0;
}
