#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

// fake malloc
void * (*ml)(size_t) = malloc;
void *fake_malloc(size_t size)
{
  if (rand() % 100000 == 0) {
    return 0;
  } else {
    return ml(size);
  }
}
#define malloc fake_malloc


struct link {
  int value;
  struct link *next;
};

struct link *new_link(int val, struct link *next)
{
  struct link *link = malloc(sizeof *link);
  if (!link) return 0;
  link->value = val;
  link->next = next;
  return link;
}

void free_links(struct link *x)
{
  while (x) {
    struct link *next = x->next;
    free(x);
    x = next;
  }
}


typedef struct link * list;
#define new_list()   new_link(0, 0)
#define free_list(x) free_links(x)

list make_list(int n, int array[n])
{
  list x = new_list();
  if (!x) return 0;

  for (int i = n - 1; i >= 0; i--) {
    struct link *link = new_link(array[i], x->next);
    if (!link) { // Allocation error -- clean up
      free_list(x);
      return 0;
    }
    x->next = link;
  }
  return x;
}

void print_list(list x)
{
  printf("[ ");
  struct link *link = x->next;
  while (link) {
    printf("%d ", link->value);
    link = link->next;
  }
  printf("]\n");
}

bool contains(list x, int val)
{
  struct link *link = x->next;
  while (link) {
    if (link->value == val)
      return true;
    link = link->next;
  }
  return false;
}

int prepend(list x, int val)
{
  struct link *link = new_link(val, x->next);
  if (!link) return 0;
  x->next = link;
  return 1;
}

struct link *last_link(list x)
{
  struct link *prev = x;
  while (prev->next) {
    prev = prev->next;
  }
  return prev;
}

int append(list x, int val)
{
  struct link *link = new_link(val, 0);
  if (!link) return 0;
  last_link(x)->next = link;
  return 1;
}

void concatenate(list x, list y)
{
  last_link(x)->next = y->next;
  y->next = 0;
}

void delete_value(list x, int val)
{
  struct link *front = x, *next = 0;
  while (front) {
    while ( (next = front->next) &&
            next->value == val ) {
      front->next = next->next;
      free(next);
    }
    front = next;
  }
}

void reverse(list x)
{
  struct link *next = x->next;
  x->next = 0;
  while (next) {
    struct link *next_next = next->next;
    next->next = x->next;
    x->next = next;
    next = next_next;
  }
}

int main(void)
{
  srand(time(0));
  int array[] = { 1, 2, 3, 4, 5 };
  int n = sizeof(array) / sizeof(array[0]);

  list x = make_list(n, array);
  if (!x) {
    perror("Make list error:");
    exit(1);
  }

  printf("Contains:\n");
  printf("%d %d %d\n",
          contains(x, 0),
          contains(x, 3),
          contains(x, 6));
  free_list(x);
  printf("\n");

  printf("prepend/append\n");
  x = make_list(n, array);
  if (!x) {
    perror("Make list error:");
    exit(1);
  }

  int success = append(x, 6);
  if (!success) {
    perror("List error:");
    exit(1);
  }
  success = prepend(x, 0);
  if (!success) {
    perror("List error:");
    exit(1);
  }
  print_list(x);
  free_list(x);
  printf("\n");

  printf("concatenate:\n");
  x = make_list(n, array);
  list y = make_list(n, array);

  concatenate(x, y);
  print_list(x);
  free_list(x);
  free_list(y);
  printf("\n");

  printf("deleting values:\n");
  x = make_list(n, array);
  print_list(x);
  delete_value(x, 2);
  delete_value(x, 3);
  print_list(x);
  free_list(x);
  printf("\n");

  printf("deleting first link:\n");
  x = make_list(n, array);
  delete_value(x, 1);
  print_list(x);
  free_list(x);
  printf("\n");

  printf("reversing:\n");
  x = make_list(n, array);
  reverse(x);
  print_list(x);
  free_list(x);
  printf("\n");

  return 0;
}
