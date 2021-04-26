#include <stdio.h>
#include <ctype.h>

char *skip_word(char *x)
{
  while (*x && isalpha(*x))
    x++;
  return x;
}

char *find_word(char *x)
{
  while (*x && !isalpha(*x))
    x++;
  return x;
}

int main(void)
{
  char const *words = "\tfoo!  bar\n\tbaz qux\n";

  char *x = find_word((char *)words);
  while (*x) {
    printf("Current position: '%s'\n", x);
    x = find_word(skip_word(x));
  }

  return 0;
}
