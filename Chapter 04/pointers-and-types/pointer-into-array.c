#include <stdio.h>
#include <assert.h>

int main(void)
{
  int a[] = { 1, 2, 3, 4, 5 };
  int n = sizeof a / sizeof *a;

  // get a pointer to the beginning of a
  int *ip = a;
  char *cp = (char *)a;

  for (int i = 0; i < n; i++) {
    printf("a[%d] sits at %p / %p / %p\n",
           i, (void *)&a[i], (void *)(ip + i),
           (void *)(cp + i * sizeof(int)));
  }

  for (int i = 0; i < n; i++) {
    // Add an integer to a pointer/array
    // to get an element at an offset
    assert(ip + i == a + i);
    // The offset is the address at
    // that index
    assert(ip + i == &a[i]);
    // Dereference and you get the value
    assert(*(ip + i) == a[i]);
    // The index operator is also valid
    // for pointers
    assert(ip[i] == a[i]);
  }

  return 0;
}
