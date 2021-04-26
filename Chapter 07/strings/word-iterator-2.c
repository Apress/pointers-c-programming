#include <stdio.h>
#include <ctype.h>


// NULL pointer instead of pointer to zero terminal
#define NULLIFY(x) ((*x) ? x : 0)

#define GEN_FIND_SKIP_ITER(name, find, skip)   \
char *first_##name(char *x)                    \
{                                              \
  return NULLIFY((find)(x));                   \
}                                              \
char *next_##name(char *x)                     \
{                                              \
  return NULLIFY((find)((skip)(x)));           \
}

#define GEN_FIND_SKIP(name, type)              \
char *skip_##name(char *x)                     \
{                                              \
  while (*x && (type)(*x))                     \
    x++;                                       \
  return x;                                    \
}                                              \
char *find_##name(char *x)                     \
{                                              \
  while (*x && !(type)(*x))                    \
    x++;                                       \
  return x;                                    \
}

#define GEN_ITER(name, type)                       \
GEN_FIND_SKIP(name, type)                          \
GEN_FIND_SKIP_ITER(name, find_##name, skip_##name)

GEN_ITER(word, isalpha)
GEN_ITER(integer, isnumber)


int main(void)
{
  char const *x = "\tfoo! 1231 bar\n\tbaz qux12\n";

  for (char *iter = first_word((char *)x);
       iter;
       iter = next_word(iter)) {
    printf("Current position: '%s'\n", iter);
  }

  for (char *iter = first_integer((char *)x);
       iter;
       iter = next_integer(iter)) {
    printf("Current position: '%s'\n", iter);
  }

  return 0;
}
