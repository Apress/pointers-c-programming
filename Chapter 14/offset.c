#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


#include "list.h"

typedef struct double_link {
  link forward_link;
  link backward_link;
  int value;
} dlink;

dlink *new_dlink(int value)
{
  dlink *link = malloc(sizeof *link);
  if (link) link->value = value;
  return link;
}

#include <stddef.h>
#define container(p,type,member) \
  (type *)((char *)p - offsetof(type, member))

void print_dlink(dlink *link)
{
  printf("%d", link->value);
}

void print_forward(link *link)
{
  print_dlink(container(link, dlink, forward_link));
}
void print_backward(link *link)
{
  print_dlink(container(link, dlink, backward_link));
}

void free_dlink(dlink *link)
{
  // We have to unlink from both lists
  // before we can safely free the link.
  unlink(&link->forward_link);
  unlink(&link->backward_link);
  free(link);
}
void free_forward(link *link)
{
  free_dlink(container(link, dlink, forward_link));
}
void free_backward(link *link)
{
  free_dlink(container(link, dlink, backward_link));
}

list_type forward_type = {
  .free  = free_forward,
  .print = print_forward
};
list_type backward_type = {
  .free  = free_backward,
  .print = print_backward
};

bool is_forward_even(link *l)
{
  dlink *link = container(l, dlink, forward_link);
  return link->value % 2 == 0;
}

int main(void)
{
  list *forward = new_list(forward_type);
  list *backward = new_list(backward_type);
  if (!forward || !backward) abort(); // error handling

  for (int i = 0; i < 10; i++) {
    dlink *link = new_dlink(i);
    if (!link) abort();
    append(forward, &link->forward_link);
    prepend(backward, &link->backward_link);
  }
  print_list(forward);
  print_list(backward);

  // Try changing the first link in forward...
  dlink *link = container(front(forward), dlink, forward_link);
  link->value = 42;
  // Now both lists have changed (because it is the same link)
  print_list(forward);
  print_list(backward);

  // delete even numbers...
  delete_if(forward, is_forward_even);
  // removes them from both lists
  print_list(forward);
  print_list(backward);

  free_list(forward);
  free_list(backward);

  return 0;
}
