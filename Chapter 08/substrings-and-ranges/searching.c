#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "substr.h"

int main(void)
{
  printf("finding patterns\n");
  do {
    char x[] = "foobarbazqux";
    substr_iter iter = as_substr(x);
    substr ba = as_substr("ba");
    for (substr word = next_occurrence(&iter, ba, 0);
         !null_substr(word);
         word = next_occurrence(&iter, ba, 0)) {

      print_substr(word); printf("\n");
    }
    printf("\n");

  } while(0);

  return 0;
}
