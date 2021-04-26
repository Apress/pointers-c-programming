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

// NULL pointer instead of pointer to zero terminal
#define NULLIFY(x) ((*x) ? x : 0)
#define WORD_ITER_END 0

char *first_word(char *x)
{
  return NULLIFY(find_word(x));
}

char *next_word(char *x)
{
  return NULLIFY(find_word(skip_word(x)));
}

char *find_integer(char *x)
{
  while (*x && !isnumber(*x))
    x++;
  return x;
}

char *skip_integer(char *x)
{
  while (*x && isnumber(*x))
    x++;
  return x;
}


char *first_integer(char *x)
{
  return NULLIFY(find_integer(x));
}

char *next_integer(char *x)
{
  return NULLIFY(find_integer(skip_integer(x)));
}

#define INTEGER_ITER_END 0

int main(void)
{
  char const *words = "\tfoo! 1231 bar\n\tbaz qux12\n";

  for (char *iter = first_word((char *)words);
       iter != WORD_ITER_END;
       iter = next_word(iter)) {
    printf("Current position: '%s'\n", iter);
  }

  for (char *iter = first_integer((char *)words);
       iter != INTEGER_ITER_END;
       iter = next_integer(iter)) {
    printf("Current position: '%s'\n", iter);
  }

  return 0;
}
