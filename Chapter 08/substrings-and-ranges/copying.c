#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "substr.h"

int main(void)
{

  printf("copying\n");
  do {
    char w[] = "foobar";
    substr x = slice(w, 0, 3);
    substr y = slice(w, 0, 3);
    substr z = copy_substr(x, y);
    assert(substr_len(z) == 0); // we copied all, so it should be empty
    printf("%s\n", z.end); // the rest of w, so bar
    assert(strcmp(z.end, "bar") == 0);
  } while(0);
  do {
    char w[] = "foobar";
    substr x = slice(w, 0, 6);
    substr y = slice(w, 3, 6);
    substr z = copy_substr(x, y);
    assert(substr_len(z) == 3); // bar is still there
    assert(substr_cmp(z, as_substr("bar")) == 0);
    printf("%s\n", w); // barbar
    assert(strcmp(w, "barbar") == 0);
    print_substr(z); printf("\n"); // bar
  } while(0);
  do {
    char w[] = "foobar";
    substr x = slice(w, 0, 2);
    substr y = slice(w, 3, 6);
    substr z = copy_substr(x, y);
    assert(substr_len(z) == 0); // we filled the input
    printf("%s\n", w); // baobar
    assert(strcmp(w, "baobar") == 0);
  } while(0);
  printf("\n");

  do {
    char w[] = "foobar";
    substr x = slice(w, 0, 3);
    substr y = slice(w, 1, 4);
    substr z = copy_substr(x, y);
    assert(substr_len(z) == 0); // we filled the input
    printf("%s\n", w); // oobbar
    assert(strcmp(w, "oobbar") == 0);
  } while(0);
  do {
    char w[] = "foobar";
    substr x = slice(w, 1, 4);
    substr y = slice(w, 0, 3);
    substr z = copy_substr(x, y);
    assert(substr_len(z) == 0); // we filled the input
    printf("%s\n", w); // ffooar
    assert(strcmp(w, "ffooar") == 0);
  } while(0);
  printf("\n");

  printf("swapping\n");
  do {
    char w[] = "foobar";
    substr x = slice(w, 0, 3);
    substr y = slice(w, 3, 6);
    swap_substr(x, y);
    printf("%s\n", w); // barfoo
  } while(0);
  printf("\n");

  return 0;
}
