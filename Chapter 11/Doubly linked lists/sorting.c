#include "list.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>

bool is_sorted(list x)
{
  struct link *p = front(x);
  while (p->next != x) {
    if (p->value > p->next->value)
      return false;
    p = p->next;
  }
  return true;
}

// Selection sort ------------------------------
struct link *get_smallest(list x)
{
  assert(!is_empty(x));
  struct link *p = front(x);
  struct link *res = p;
  while (p != x) {
    if (p->value < res->value)
      res = p;
    p = p->next;
  }
  return res;
}

void move_links(list x, list y)
{
  free_links(x);
  if (!is_empty(y)) {
    *x = *y;
    connect_neighbours(x);
    clear_list(y);
  }
}

void selection_sort(list x)
{
  list_head sorted = init_list_head(sorted);
  while (!is_empty(x)) {
    struct link *next = get_smallest(x);
    unlink(next);
    append_link(&sorted, next);
  }
  move_links(x, &sorted);
}


// Insertion sort -----------------------------------------------
#if 0
// Insert from the beginning. Not the usual insertion sort. Not stable
void insert_sorted(list x, struct link *link)
{
  struct link *p = front(x);
  while (p != x && p->value < link->value)
    p = p->next;
  link_before(p, link);
}

#else
// Insert from the end; this is stable
void insert_sorted(list x, struct link *link)
{
  struct link *p = last(x);
  while (p != x && p->value > link->value)
    p = p->prev;
  link_after(p, link);
}
#endif

#if 0
void insertion_sort(list x)
{
  list_head sorted = init_list_head(sorted);
  struct link *p = front(x);
  while (p != x) {
    struct link *next = p->next;
    unlink(p);
    insert_sorted(&sorted, p);
    p = next;
  }
  move_links(x, &sorted);
}
#else
void insertion_sort(list x)
{
  list_head sorted = init_list_head(sorted);
  while (!is_empty(x)) {
    struct link *p = front(x);
    unlink(p);
    insert_sorted(&sorted, p);
  }
  move_links(x, &sorted);
}
#endif

// Merge sort -----------------------------------------------
#if 0
void merge(list x, list y)
{
  list_head merged = init_list_head(merged);
  struct link *p = front(x), *q = front(y);

  while( (p != x) && (q != y) ) {
    struct link *smallest;
    if (p->value < q->value) {
      unlink(p);
      smallest = p; p = p->next;
    } else {
      unlink(q);
      smallest = q; q = q->next;
    }
    append_link(&merged, smallest);
  }

  concatenate(&merged, x);
  concatenate(&merged, y); // also clears y
  move_links(x, &merged);
}
#else
void merge(list x, list y)
{
  list_head merged = init_list_head(merged);
  while( !is_empty(x) && !is_empty(y) ) {
    struct link *smallest =
      (front(x)->value < front(y)->value)
      ? front(x) : front(y);
    unlink(smallest);
    append_link(&merged, smallest);
  }
  concatenate(&merged, x);
  concatenate(&merged, y); // also clears y
  move_links(x, &merged);
}
#endif

#if 1
void split_list(list x, list y)
{
  assert(is_empty(y));
  struct link *p = front(x);
  while (p != x) {
    struct link *q = p->next;
    unlink(p); append_link(y, p);
    if (q == x) return;
    p = q->next;
  }
}
#else
void split_list(list x, list y)
{
  assert(is_empty(y));

  list_head z = init_list_head(z);
  bool flag = true;
  while (!is_empty(x)) {
    list target = ((flag = !flag)) ? y : &z;
    struct link *p = front(x);
    unlink(p);
    append_link(target, p);
  }

  move_links(x, &z);
}
#endif

void merge_sort(list x)
{
  if (is_empty(x) || front(x)->next == x)
    return; // length zero or one lists are sorted

  list_head y = init_list_head(y);
  split_list(x, &y);
  merge_sort(x); merge_sort(&y);
  merge(x, &y);
}

// Quick sort -----------------------------------------------
void partition(list x, list y, int pivot)
{
  assert(is_empty(y));
  struct link *p = front(x);
  while (p != x) {
    struct link *next = p->next;
    if (p->value > pivot) {
      unlink(p); append_link(y, p);
    }
    p = next;
  }
}

void quick_sort(list x)
{
  if (is_empty(x) || front(x)->next == x)
    return; // length zero or one lists are sorted

  // remove the pivot, to make sure that we reduce
  // the problem size each recursion
  struct link *first = front(x); unlink(first);
  int pivot = first->value;

  list_head y = init_list_head(y);
  partition(x, &y, pivot);
  quick_sort(x); quick_sort(&y);
  append_link(x, first); // get first back into the list
  concatenate(x, &y);
}



list random_list(int n)
{
  list x = new_list();
  if (!x) abort();

  for (int i = 0; i < n; i++) {
    if (!append(x, rand() % 10)) abort();
  }

  return x;
}

void test_sorting(int n)
{
  list x = random_list(n);
  printf("x:         "); print_list(x);

  list y = copy_list(x);
  selection_sort(y);
  printf("selection: "); print_list(y);
  assert(is_sorted(y));
  free_list(y);

  y = copy_list(x);
  insertion_sort(y);
  printf("insertion: "); print_list(y);
  assert(is_sorted(y));
  free_list(y);

  y = copy_list(x);
  merge_sort(y);
  printf("merge:     "); print_list(y);
  assert(is_sorted(y));
  free_list(y);

  y = copy_list(x);
  quick_sort(y);
  printf("quick:     "); print_list(y);
  assert(is_sorted(y));
  free_list(y);

  printf("\n");
  free_list(x);
}

int main(void)
{
  int seed = time(NULL);
  srand(seed);

  for (int i = 0; i < 5; i++) {
    test_sorting(10);
  }

  return 0;
}
