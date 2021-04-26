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

void free_list(struct link *list)
{
  while (list) {
      // Remember next, we cannot get it
      // after free(list)
      struct link *next = list->next;
      free(list);
      list = next;
  }
}

struct link *make_list(int n, int array[n])
{
  struct link *list = 0;
  for (int i = n - 1; i >= 0; i--) {
    struct link *link = new_link(array[i], list);
    if (!link) { // Allocation error -- clean up
      free_list(list);
      return 0;
    }
    list = link;
  }
  return list;
}

void print_list(struct link *list)
{
  printf("[ ");
  while (list) {
      printf("%d ", list->value);
      list = list->next;
  }
  printf("]\n");
}


bool contains(struct link *list, int val)
{
  while (list) {
      if (list->value == val)
          return true;
      list = list->next;
  }
  return false;
}

struct link *prepend(struct link *list, int val)
{
  struct link *new_list = new_link(val, list);
  if (!new_list) free_list(list);
  return new_list;
}

struct link *append(struct link *list, int val)
{
  struct link *val_link = new_link(val, 0);
  if (!val_link) {
    free_list(list);
    return 0;
  }
  
  if (!list) return val_link;

  struct link *last = list;
  while (last->next) {
      last = last->next;
  }
  last->next = val_link;
  return list;
}

struct link *concatenate(struct link *x, struct link *y)
{
  if (!x) return y;
  struct link *last = x;
  while (last->next) {
      last = last->next;
  }
  last->next = y;
  return x;
}

struct link *append_(struct link *list, int val)
{
  return concatenate(list, new_link(val, 0));
}

struct link *delete_value(struct link *list, int val)
{
  if (!list) return 0;
  if (list->value == val) {
      struct link *next = list->next;
      free(list);
      return delete_value(next, val);
  } else {
      list->next = delete_value(list->next, val);
      return list;
  }
}

struct link *reverse(struct link *list)
{
  if (!list) return 0;
  struct link *next = list->next;
  struct link *reversed = list; reversed->next = 0;
  while (next) {
      struct link *next_next = next->next;
      next->next = reversed;
      reversed = next;
      next = next_next;
  }
  return reversed;
}

int main(int argc, char **argv)
{
  srand(time(0));
  int array[] = { 1, 2, 3, 4, 5 };
  int n = sizeof array / sizeof *array;

  struct link *list = make_list(n, array);
  if (!list) {
    perror("List error: ");
    exit(1); // Just bail here
  }

  printf("Contains:\n");
  printf("%d %d %d\n",
          contains(list, 0),
          contains(list, 3),
          contains(list, 6));
  free_list(list);
  printf("\n");

  printf("prepend/append\n");
  list = make_list(n, array);
  if (!list) {
    perror("List error: ");
    exit(1); // Just bail here
  }

  // This is the natural way to write code...
  list = append(list, 6);
  list = prepend(list, 0);

  // The error handling forces us to keep the old
  // list around
  struct link *new_list = append(list, 6);
  if (!list) {
    perror("List error: ");
    exit(1); // Just bail here
  }
  list = new_list;
  new_list = prepend(list, 0);
  if (!list) {
    perror("List error: ");
    exit(1); // Just bail here
  }
  list = new_list;

  print_list(list);
  free_list(list);
  printf("\n");

  printf("memory issues...\n");
  list = make_list(n, array);
  new_list = prepend(list, -1);
  if (!new_list) exit(1);
  struct link *list2 = new_list;

  print_list(list);
  print_list(list2);

  free_list(list);
  // WARNING WARNING WARNING!
  // Now, list2->next points to garbage!
  free(list2); // but we can delete just the link...
  printf("\n");


  printf("concatenate:\n");
  list = make_list(n, array);
  if (!list) {
    perror("List error: ");
    exit(1);
  }
  list2 = make_list(n, array);
  if (!list2) {
    perror("List error: ");
    exit(1);
  }

  struct link *list3 = concatenate(list, list2);
  // list and list3 are the same now
  print_list(list);
  print_list(list3);
  free_list(list3); // this also deletes list and list2!
  // If we had deleted list2, we couldn't delete list/list2 now
  printf("\n");

  printf("alternative append:\n");
  list = make_list(n, array);
  if (!list) {
    perror("List error: ");
    exit(1);
  }

  list = append_(list, 6);
  if (!list) {
    perror("List error: ");
    exit(1);
  }

  print_list(list);
  free_list(list);
  printf("\n");

  printf("deleting values:\n");
  list = make_list(n, array);
  if (!list) {
    perror("List error: ");
    exit(1);
  }
  list = delete_value(list, 2);
  list = delete_value(list, 3);
  print_list(list);
  free_list(list);
  printf("\n");

  printf("deleting first link:\n");
  list = make_list(n, array);
  list2 = delete_value(list, 1);
  print_list(list2);

  // the first link in list was freed, so
  // we cannot use it.
  // This would be an error: print_list(list);

  free_list(list2);
  // Even if we hadn't deleted the first link in
  // delete_value(), we would have deleted the rest
  // of the list now, so free_list(list) isn't possible
  printf("\n");

  printf("reversing:\n");
  list = make_list(n, array);
  list = reverse(list);
  print_list(list);
  free_list(list);
  printf("\n");

  return EXIT_SUCCESS;
}
