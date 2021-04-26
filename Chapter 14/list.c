#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "list.h"

list *new_list(list_type type)
{
  list *list = malloc(sizeof *list);
  if (list) {
    *list = (struct list){
      .head = { .next = &list->head,
                .prev = &list->head },
      .type = type
    };
  }
  return list;
}


void free_list(list *x)
{
  void (*free_link)(link *) = x->type.free;
  // We can only free if we have a free function.
  // Otherwise, assume that we shouldn't free.
  if (free_link) {
    link *lnk = front(x);
    while (lnk != head(x)) {
        link *next = lnk->next;
        free_link(lnk);
        lnk = next;
    }
  }
  free(x);
}

// Default print function
static void print_link(link *lnk)
{
  printf("<link %p>", (void *)lnk);
}

void print_list(list *x)
{
  void (*print)(link *) =
    (x->type.print) ? x->type.print : print_link;
  printf("[ ");
  for (link *lnk = front(x);
       lnk != head(x); lnk = lnk->next) {
    print(lnk);
    putchar(' ');
  }
  printf("]\n");
}

link *find_link(list *x, link *from, bool (*p)(link *))
{
  for (link *lnk = from;
       lnk != head(x);
       lnk = lnk->next) {
    if (p(lnk)) return lnk;
  }
  return 0;
}

void delete_if(list *x, bool (*p)(link *))
{
  void (*free)(link *) = x->type.free;
  link *lnk = front(x);
  while (lnk != head(x)) {
    link *next = lnk->next;
    if (p(lnk)) {
      unlink(lnk);
      if (free) free(lnk);
    }
    lnk = next;
  }
}
