#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// fake malloc
void * (*ml)(size_t) = malloc;
void *fake_malloc(size_t size)
{
  if (rand() % 100 == 0) {
    return 0;
  } else {
    return ml(size);
  }
}
#define malloc fake_malloc

// for contains
#include <stdbool.h>






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

void free_links(struct link *list)
{
  while (list) {
    struct link *next = list->next;
    free(list);
    list = next;
  }
}


// A list is a pointer to a link
typedef struct link ** list;

#define is_list_empty(x) (*(x) == 0)
#define list_links(x)     *(x)

list new_list(void)
{
  list x = malloc(sizeof *x);
  if (x) *x = 0;
  return x;
}

void free_list(list x)
{
  free_links(list_links(x));
  free(x);
}

// Same as before...
list make_list(int n, int array[n])
{
  list x = new_list();
  if (!x) return 0;

  for (int i = n - 1; i >= 0; i--) {
    struct link *link =
      new_link(array[i], list_links(x));
    if (!link) { // Allocation error -- clean up
      free_list(x);
      return 0;
    }
    list_links(x) = link;
  }

  return x;
}

void print_list(list x)
{
  printf("[ ");
  struct link *link = list_links(x);
  while (link) {
      printf("%d ", link->value);
      link = link->next;
  }
  printf("]\n");
}

bool contains(list x, int val)
{
  struct link *links = list_links(x);
  while (links) {
      if (links->value == val)
          return true;
      links = links->next;
  }
  return false;
}

int prepend(list x, int val)
{
  struct link *link =
    new_link(val, list_links(x));
  if (!link) return 0;
  list_links(x) = link;
  return 1;
}

struct link **last_next(struct link **x)
{
  if (*x == 0) return x;

  struct link *prev = *x;
  while (prev->next) {
    prev = prev->next;
  }
  return &prev->next;
}

int append(list x, int val)
{
  struct link *val_link = new_link(val, 0);
  if (!val_link) return 0;
  *last_next(x) = val_link;
  return 1;
}

void concatenate(list x, list y)
{
  *last_next(x) = list_links(y);
  // remove alias to the old y links
  *y = 0;
}

typedef struct link * stack_list;
#define stack_to_list(x) (&x)
#define free_stack_list(x) \
  do { free_links(x); x = 0; } while(0)

void copy_list(list x, list y)
{
  free_links(list_links(x));
  concatenate(x, y);
}

void delete_value(list x, int val)
{
  // We can use the previous implementation, but update
  // x...
}

void reverse(list x)
{
  // We can use the previous implementation, but update
  // x...
}

int main(int argc, char **argv)
{
  srand(time(0));
  int array[] = { 1, 2, 3, 4, 5 };
  int n = sizeof array / sizeof *array;

  list x = make_list(n, array);
  if (!x) {
    perror("List error: ");
    exit(1); // Just bail here
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
    perror("List error: ");
    exit(1); // Just bail here
  }

  // This is the natural way to write code...
  if (!append(x, 6)) { perror("Append error:"); exit(1); };
  print_list(x);
  if (!prepend(x, 0)) { perror("Prepend error:"); exit(1); };
  print_list(x);

  print_list(x);
  free_list(x);
  printf("\n");


  printf("concatenate:\n");
  x = make_list(n, array);
  if (!x) {
    perror("List error: ");
    exit(1);
  }
  list y = make_list(n, array);
  if (!y) {
    perror("List error: ");
    exit(1);
  }

  concatenate(x, y);
  // list and list3 are the same now
  print_list(x);
  print_list(y);
  free_list(x);
  free_list(y);
  printf("\n");

  printf("stack list\n");
  x = make_list(n, array);
  stack_list z = 0;
  print_list(x);
  print_list(stack_to_list(z));

  printf("copying...\n");
  copy_list(stack_to_list(z), x);
  print_list(x);
  print_list(stack_to_list(z));
  free_list(x);
  free_stack_list(z);
  printf("\n");


  return EXIT_SUCCESS;
}
