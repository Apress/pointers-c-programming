#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *find_word(char *x)
{
  while (*x && !isalpha(*x))
    x++;
  return x;
}

void copy_word(char **from, char **to)
{
  while (isalpha(**from)) {
    *(*to)++ = *(*from)++;
  }
}

void copy_words(char *from, char *to)
{
  from = find_word(from);
  while (*from) {
    copy_word(&from, &to);
    from = find_word(from);
    if (*from) *to++ = ' ';
  }
  *to = '\0';
}

void compact_words(char *s)
{
  copy_words(s, s);
}

int main(void)
{
  // We modify the string we wordize, so we
  // cannot use a literate string. Those are immutable.
  // This initialises a mutable buffer instead. The
  // string is copied into the buffer, including the zero-
  // terminal, when the buffer is initialised.
  char string[] = "\tfoo!  bar\n\tbaz qux\n";
  compact_words(string);
  printf("'%s'\n", string);

  return 0;
}
