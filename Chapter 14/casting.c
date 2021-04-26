#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"

struct int_link {
  link link;
  int value;
};
typedef struct int_link ilink;

ilink *new_int_link(int value)
{
  ilink *lnk = malloc(sizeof *lnk);
  if (lnk) lnk->value = value;
  return lnk;
}

void print_int_link(link *lnk)
{
  printf("%d", ((ilink *)lnk)->value);
}

void free_int_link(link *lnk)
{
  free(lnk); // Nothing special
}

list_type int_list = {
  .free = free_int_link,
  .print = print_int_link
};

bool is_even(link *l)
{
  ilink *link = (ilink *)l;
  return link->value % 2 == 0;
}

int main(void)
{
  list *x = new_list(int_list);
  for (int i = 0; i < 10; i++) {
    ilink *lnk = new_int_link(i);
    if (!lnk) abort();
    append(x, (link *)lnk);
  }
  print_list(x);

  ilink *lnk = (ilink *)find_link(x, front(x), is_even);
  printf("%d\n", lnk->value);
  lnk = (ilink *)find_link(x, lnk->link.next, is_even);
  printf("%d\n", lnk->value);

  for (link *lnk = find_link(x, front(x), is_even);
       lnk;
       lnk = find_link(x, lnk->next, is_even)) {
    printf("%d ", ((ilink *)lnk)->value);
  }
  printf("\n");

  delete_if(x, is_even);
  print_list(x);

  free_list(x);

  // using stack-allocated links
  ilink l1 = { .value = 13 };
  ilink l2 = { .value = 42 };
  struct list_type type = {
    .print = print_int_link,
    .free = 0 // Do not free stack allocated links
  };
  x = new_list(type);
  append(x, (struct link *)&l1);
  append(x, (struct link *)&l2);
  print_list(x);
  free_list(x);

  return 0;
}
