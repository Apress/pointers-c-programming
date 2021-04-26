#include <stdio.h>
#include <ctype.h>


char *find(int (*char_class)(int), char *x)
{
  while (*x && !char_class(*x))
    x++;
  return x;
}

char *skip(int (*char_class)(int), char *x)
{
  while (*x && char_class(*x))
    x++;
  return x;
}

typedef struct {
  char *x;
  int (*char_class)(int);
} find_skip_iter;

#define NULLIFY(x) ((*x) ? x : 0)

char *init_iter(find_skip_iter *iter, char *x,
                int (*char_class)(int))
{
  iter->char_class = char_class;
  iter->x = find(iter->char_class, x);
  return NULLIFY(iter->x);
}

char *iter_next(find_skip_iter *iter)
{
  iter->x = skip(iter->char_class, iter->x);
  iter->x = find(iter->char_class, iter->x);
  return NULLIFY(iter->x);
}

#define init_word_iter(itr, x) \
        init_iter((itr),(x), isalpha)
#define init_int_iter(itr, x) \
        init_iter((itr),(x), isnumber)

int main(void)
{
  char *x = "123 sss 321 xxx 123";

  find_skip_iter itr;
  for (char *y = init_word_iter(&itr, x);
       y; y = iter_next(&itr)) {
    printf("%s\n", y);
  }

  for (char *y = init_int_iter(&itr, x);
       y; y = iter_next(&itr)) {
    printf("%s\n", y);
  }

  return 0;
}
