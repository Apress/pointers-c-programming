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

void list_cleanup(struct link **x)
  { decref(*x); }
#define autoclean_list \
  list __attribute__((cleanup(list_cleanup)))

list cons(int head, takes list tail_)
{
  autoclean_list tail  = tail_;

  if (is_error(tail)) return 0;
  list new_link = malloc(sizeof *new_link);
  if (new_link) {
    struct link link_data = {
      .refcount = 1,
      .next  = incref(tail), // tail will be deleted, so remember it here
      .value = head
    };
    memcpy(new_link, &link_data, sizeof *new_link);
  }
  return give(new_link);
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
int length_rec(takes list x_, int acc)
{
  autoclean_list x = x_;
  if (is_nil(x)) {
    return acc;
  } else {
    return length_rec(incref(x->next), acc + 1);
  }
}
#define length(x) length_rec(x, 0)


// REVERSE --------------------------------------
list reverse_rec(takes list x_, takes list acc_)
{
  autoclean_list x = x_, acc = acc_;
  if (is_error(x) || is_error(acc)) return 0;
  if (is_nil(x)) {
    return incref(acc);
  } else {
    return reverse_rec(incref(x->next),
                       cons(x->value, incref(acc)));
  }
}

#define reverse(x) reverse_rec(x, NIL)


// CONCAT --------------------------------------
list concat(takes list x_, takes list y_)
{
  autoclean_list x = x_, y = y_;
  if (is_error(x) || is_error(y)) return 0;
  if (is_nil(x)) {
    return incref(y);
  } else {
    return cons(x->value, concat(incref(x->next), incref(y)));
  }
}



int main(void)
{
  printf("CONSTRUCTION\n");
  list x = cons(1, cons(2, cons(3, NIL)));
  print_list(x);

  list y = cons(0, incref(x));
  print_list(y);

  decref(x);
  print_list(y);

  list z = cons(-1, give(y)); // consider y gone!
  print_list(z);
  decref(z);


  printf("LENGTH\n");
  x = cons(1, cons(2, cons(3, NIL)));
  print_list(x);
  printf("len(x) == %d\n", length(incref(x)));
  print_list(x);
  // leaks if length doesn't free its input
  printf("len(x) == %d\n", length(cons(1, cons(2, cons(3, NIL)))));

  //decref(x);
  // this frees x
  length(give(x));


  printf("REVERSE\n");

  x = cons(1, cons(2, cons(3, NIL)));
  print_list(x);
  y = reverse(give(x));
  print_list(y);
  decref(y);

  // would leak if reverse and length didn't free their input
  printf("%d\n", length(reverse(cons(1, cons(2, cons(3, NIL))))));


  printf("CONCAT\n");
  x = cons(1, cons(2, cons(3, NIL)));
  y = cons(4, cons(5, NIL));
  z = concat(incref(x), incref(y));

  print_list(z);
  decref(x); decref(y);
  print_list(z);
  decref(z);

  z = concat(cons(1, cons(2, NIL)), cons(11, cons(12, cons(13, NIL))));
  print_list(z);
  decref(z);


  return 0;
}
