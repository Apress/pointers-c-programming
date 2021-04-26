#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct link {
  int refcount;
  struct link * const next;
  int           const value;
};
typedef struct link *list;

struct link *incref(struct link *link)
{
  link->refcount++;
  return link;
}

void free_link(struct link *link);
struct link *decref(struct link *link)
{
  if (link && --link->refcount == 0) {
    free_link(link);
    return 0;
  }
  return link;
}

void free_link(struct link *link)
{
  decref(link->next);
  free(link);
}

// Empty lists and errors
#define is_error(x) ((x) == 0)
struct link NIL_LINK = { .refcount = 1 };
#define is_nil(x)   ((x) == &NIL_LINK)
#define NIL incref(&NIL_LINK)

// Give a reference; you lose it yourself
#define takes
#define give(x) x

#if 0
// GCC compiler extension statement-expression for give()
#define give(x)  ({ void *p = x; x = 0; p; })
#endif

// Let the function use your reference
#define borrows


list new_link(int head, takes list tail)
{
  if (is_error(tail)) return tail;

  list link = malloc(sizeof *link);
  if (!link) { decref(tail); return 0; }

  struct link link_data = {
    .refcount = 1,
    .next  = give(tail), // gives away the reference
    .value = head
  };
  memcpy(link, &link_data, sizeof *link);
  return give(link);
}

void print_list(borrows list x)
{
  printf("[ ");
  while (!is_nil(x)) {
    printf("%d[%d] ", x->value, x->refcount);
    x = x->next;
  }
  printf("]\n");
}

// LENGTH ------------------------------------------
#if 0
int length_rec(borrows list x, int acc)
{
  assert(!is_error(x));
  if (is_nil(x))
    return acc;
  else
    return length_rec(x->next, acc + 1);
}
#define length(x) length_rec(x, 0)

#else

int length_rec(takes list x, int acc)
{
  assert(!is_error(x));
  if (is_nil(x)) {
    decref(x);
    return acc;

  } else {
    struct link *next = incref(x->next);
    decref(x);
    return length_rec(give(next), acc + 1);
  }
}
#define length(x) length_rec(x, 0)

#endif

// REVERSE --------------------------------------
#if 0
list reverse_rec(borrows list x, borrows list acc)
{
  if (is_error(x) || is_error(acc)) {
    return 0;
  }
  if (is_nil(x)) {
    return incref(acc);
  } else {
    return reverse_rec(x->next, new_link(x->value, incref(acc)));
  }
}

#define reverse(x) reverse_rec(x, NIL)

#else

list reverse_rec(takes list x, takes list acc)
{
  if (is_error(x) || is_error(acc)) {
    decref(x); decref(acc);
    return 0;
  }
  if (is_nil(x)) {
    decref(x);
    return give(acc);

  } else {
    int value = x->value;
    struct link *next = incref(x->next);
    decref(x);
    return reverse_rec(give(next), new_link(value, acc));
  }
}

#define reverse(x) reverse_rec(x, NIL)

#endif

// CONCAT --------------------------------------
#if 0
list concat(borrows list x, borrows list y)
{
  if (is_error(x) || is_error(y)) {
    return 0;
  }
  if (is_nil(x)) {
    return incref(y); // we return a new ref, so we must incref here
  } else {
    return new_link(x->value, concat(x->next, incref(y)));
  }
}

#else

list concat(takes list x, takes list y)
{
  if (is_error(x) || is_error(y)) {
    decref(x); decref(y);
    return 0;
  }
  if (is_nil(x)) {
    decref(x);
    return give(y);
  } else {
    int value = x->value;
    struct link *next = incref(x->next);
    decref(x);
    return new_link(value, concat(give(next), give(y)));
  }
}


#endif


int main(void)
{
  printf("CONSTRUCTION\n");
  list x = new_link(1, new_link(2, new_link(3, NIL)));
  print_list(x);

  list y = new_link(0, incref(x));
  print_list(y);

  decref(x);
  print_list(y);

  list z = new_link(-1, give(y)); // consider y gone!
  print_list(z);
  decref(z);


  printf("LENGTH\n");
  x = new_link(1, new_link(2, new_link(3, NIL)));
  print_list(x);
  printf("len(x) == %d\n", length(incref(x)));
  print_list(x);
  // leaks if length doesn't free its input
  printf("len(x) == %d\n", length(new_link(1, new_link(2, new_link(3, NIL)))));

  //decref(x);
  // this frees x
  length(give(x));


  printf("REVERSE\n");

  x = new_link(1, new_link(2, new_link(3, NIL)));
  print_list(x);
  y = reverse(give(x));
  print_list(y);
  decref(y);

  // would leak if reverse and length didn't free their input
  printf("%d\n", length(reverse(new_link(1, new_link(2, new_link(3, NIL))))));


  printf("CONCAT\n");
  x = new_link(1, new_link(2, new_link(3, NIL)));
  y = new_link(4, new_link(5, NIL));
  z = concat(incref(x), incref(y));

  print_list(z);
  decref(x); decref(y);
  print_list(z);
  decref(z);

  z = concat(new_link(1, new_link(2, NIL)), new_link(11, new_link(12, new_link(13, NIL))));
  print_list(z);
  decref(z);


  return 0;
}
