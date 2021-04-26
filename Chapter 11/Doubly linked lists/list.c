#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "list.h"

struct link *new_link(int val,
                      struct link *prev,
                      struct link *next)
{
  struct link *link = malloc(sizeof *link);
  if (!link) return 0;

  link->value = val;
  link->prev = prev;
  link->next = next;
  return link;
}


list new_list(void)
{
  struct link *head = new_link(0, 0, 0);
  if (!head) return 0;
  *head = init_list_head(*head);
  return head;
}


void free_links(list_head *head)
{
  struct link *link = front(head);
  while (link != head) {
      struct link *next = link->next;
      free(link);
      link = next;
  }
  clear_list(head);
}

int insert_val_after(struct link *after, int val)
{
  struct link *link =
    new_link(val, after, after->next);
  if (!link) return 0;
  connect_neighbours(link);
  return 1;
}

list make_list(int n, int array[n])
{
  list x = new_list();
  if (!x) return 0;

  // going in the forward direction and appending
  for (int i = 0; i < n; i++) {
    if (!append(x, array[i])) {
      free_list(x);
      return 0;
    }
  }

  return x;
}

void print_list(list x)
{
  printf("[ ");
  for (struct link *link = front(x);
       link != x;
       link = link->next) {
    printf("%d ", link->value);
  }
  printf("]\n");
}

bool contains(list x, int val)
{
  for (struct link *link = front(x);
       link != x;
       link = link->next) {

    if (link->value == val)
      return true;

  }
  return false;
}


// We don't delete y, but we empty it.
// The caller must free it if he no longer
// needs it. We could free it here, that
// just changes the API. It is a design
// choice.
void concatenate(list x, list y)
{
  connect(last(x), front(y));
  connect(last(y), x);
  clear_list(y);
}

void delete_value(list x, int val)
{
  struct link *link = front(x);
  while (link != x) {
    struct link *next = link->next;
    if (link->value == val)
      delete_link(link);
    link = next;
  }
}



#if 0
#define swap_int(x,y) \
  do { int tmp = x; x = y; y = tmp; } while(0)

void reverse(list x)
{
  struct link *left = front(x);
  struct link *right = last(x);
  while (left != right) {
    swap_int(left->value, right->value);
    left = left->next; right = right->prev;
  }
}

#else

#define swap_p(x,y) \
  do { struct link *tmp = x; x = y; y = tmp; } while(0)

void reverse(list x)
{
  struct link *p = x;
  do {
    swap_p(p->prev, p->next);
    p = p->prev;
  } while (p != x);
}

#endif


list copy_list(list x)
{
  list res = new_list();
  if (!res) return 0;

  for (struct link *p = front(x);
       p != x; p = p->next) {

    if (!append(res, p->value)) {
      free_list(res);
      return 0;
    }
  }

  return res;
}

bool equal(list x, list y)
{
  struct link *p = front(x);
  struct link *q = front(y);
  while ( (p != x) && (q != y) ) {
    if (p->value != q->value)
      return false;
    p = p->next; q = q->next;
  }
  return (p == x) && (q == y);
}
