
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "substr.h"

int main(void)
{
  printf("iterating over words\n");
  char x[] = "\tfoo    bar123baz\nqux321";
  substr_iter iter = as_substr(x);
  for (substr word = next_word(&iter);
       !null_substr(word);
       word = next_word(&iter)) {

    print_substr(word); printf("\n");

    char c = zero_term(word);
    printf("%s\n", word.begin);
    restore_term(word, c);
  }
  printf("\n");

  substr xx = compact_words(as_substr(x));
  char c = zero_term(xx);
  printf("compacted words: \"%s\"\n", x);
  restore_term(xx, c);

  return 0;
}
