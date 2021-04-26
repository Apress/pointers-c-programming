#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct link {
  struct link *prev;
  struct link *next;
} link;

static inline
void connect(link *x, link *y)
{ x->next = y; y->prev = x; }

static inline
void connect_neighbours(link *x)
{ x->next->prev = x; x->prev->next = x; }

static inline
void link_after(link *x, link *y)
{
  y->prev = x; y->next = x->next;
  connect_neighbours(y);
}
#define link_before(x, y) link_after((x)->prev, y)

// This time, unlink will set x's pointers to NULL.
// We don't want to risk the callback function modifying
// the list after the link is removed.
static inline
void unlink(link *x)
{
  if (!x->prev || !x->next) return;
  x->next->prev = x->prev;
  x->prev->next = x->next;
  x->prev = x->next = 0;
}

typedef struct list_type {
  void  (*free)(link *);
  void (*print)(link *);
} list_type;
typedef struct list {
  link head;
  list_type type;
} list;

#define head(x)     (&(x)->head)
#define front(x)    (head(x)->next)
#define back(x)     (head(x)->prev)
#define is_empty(x) (head(x) == front(x))

#define append(x,link)   link_before(head(x), link)
#define prepend(x, link) link_after(head(x), link)

list  *new_list(list_type type);

void  free_list(list *x);
void print_list(list *x);

link *find_link(list *x, link *from, bool (*p)(link *));
void  delete_if(list *x, bool (*p)(link *));


#endif // LIST_H
