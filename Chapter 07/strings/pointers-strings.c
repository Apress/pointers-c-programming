#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void)
{
  char const *string = "hello, world!\n";
  char const *cp = string;
  int n = strlen(string);
  for (int i = 0; i < n; i++) {
    assert(string[i] == cp[i]);
    assert(string + i == cp + i);
    assert(*(string + i) == *(cp + i));
  }

  return 0;
}
