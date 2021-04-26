#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define abort_append(x,v)       \
  do {                          \
    if (!append((x), (v))) {    \
      free_list(x); return 0;   \
    }                           \
  } while(0)

#define for_each(var,x)              \
  for (struct link *var = front(x);  \
       var != x; var = var->next)

list map(list x, int (*f)(int))
{
  list y = new_list();
  if (!y) return 0;
  for_each(link, x) {
    abort_append(y, f(link->value));
  }
  return y;
}

int add2(int x) { return 2 + x; }

list filter(list x, bool (*p)(int))
{
  list y = new_list();
  if (!y) return 0;
  for_each(link, x) {
    if (p(link->value))
      abort_append(y, link->value);
  }
  return y;
}

bool is_even(int i) { return i % 2 == 0; }

int fold(list x, int (*f)(int, int), int x0)
{
  list y = new_list();
  if (!y) return 0;
  int res = x0;
  for_each(link, x) {
    res = f(res, link->value);
  }
  return res;
}

int add(int x, int y) { return x + y; }
int sum(list x)       { return fold(x, add, 0); }

int mul(int x, int y) { return x * y; }
int prod(list x)      { return fold(x, mul, 1); }

int main(void)
{
  int a[] = { 1, 2, 3, 4, 10, 11, 12, 13, 14 };
  int n = sizeof a / sizeof *a;

  list x = make_list(n, a);
  print_list(x);

  list y = map(x, add2);
  print_list(y);

  list z = filter(x, is_even);
  print_list(z);

  printf("sum %d, prod %d\n", sum(z), prod(z));

  free_list(x);
  free_list(y);
  free_list(z);

  return 0;
}
