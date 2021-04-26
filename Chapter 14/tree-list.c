#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stddef.h>
#define container(p,type,member) \
  ((type *)((char *)p - offsetof(type, member)))

#include "list.h"
#include "stree.h"

typedef struct ordered_string {
  node node; link link;
  char const *str;
} ostring;

void print_ordered_string(ostring *str)
{
  printf("\"%s\"", str->str);
}
void free_ordered_string(ostring *str)
{
  // Remove from data structures...
  unlink(&str->link);
  remove_node(&str->node);
  // and then free...
  free(str);
}

// Tree interface...
void const *strnode_key(node *n)
  { return container(n, ostring, node)->str; }
int strnode_cmp(void const *x, void const *y)
  { return strcmp(x, y); }
void strnode_print(node *n)
  { print_ordered_string(container(n, ostring, node)); }
void strnode_free(node *n)
  { free_ordered_string(container(n, ostring, node)); }

stree_type strnode_type = {
  .key   = strnode_key,
  .cmp   = strnode_cmp,
  .print = strnode_print,
  .free  = strnode_free
};

// List interface...
void strlink_print(link *lnk)
  { print_ordered_string(container(lnk, ostring, link)); }
void strlink_free(link *lnk)
  { free_ordered_string(container(lnk, ostring, link)); }

list_type strlink_type = {
  .print = strlink_print,
  .free  = strlink_free
};

ostring *new_ostring(char const *str)
{
  ostring *n = malloc(sizeof *n);
  if (!n) abort();
  n->str = str;
  return n;
}

typedef struct ordered_strings {
  stree *map; list *order;
} ordered_strings;

ordered_strings *new_ordered_strings(void)
{
  ordered_strings *os = malloc(sizeof *os);
  if (!os) abort(); // handle alloc errors
  os->map = new_tree(strnode_type);
  os->order = new_list(strlink_type);
  if (!os->map || !os->order) abort(); // handle errors
  return os;
}

void add_string(ordered_strings *os, char const *str)
{
  ostring *ostr = new_ostring(str);
  if (!ostr) abort(); // handle alloc errors
  insert_node(os->map, &ostr->node);
  append(os->order, &ostr->link);
}

void remove_string(ordered_strings *os, char const *str)
{
  node *n = find_node(os->map, str);
  if (n) {
    ostring *x = container(n, ostring, node);
    free_ordered_string(x);
  }
}

link *take_front(list *x, int idx)
{
  for (link *lnk = front(x);
       lnk != head(x); lnk = lnk->next) {
    if (idx-- == 0) return lnk;
  }
  return 0;
}

link *take_back(list *x, int idx)
{
  for (link *lnk = back(x);
       lnk != head(x); lnk = lnk->prev) {
    if (idx-- == 0) return lnk;
  }
  return 0;
}

void remove_index(ordered_strings *os, int idx)
{
  link *lnk;
  if (idx < 0) {
    lnk = take_back(os->order, -idx - 1);
  } else {
    lnk = take_front(os->order, idx);
  }

  if (!lnk) {
    // report an error...
    return;
  }
  ostring *x = container(lnk, ostring, link);
  free_ordered_string(x);
}

void free_ordered_strings(ordered_strings *os)
{
  // Freeing through the tree, and unlinking through
  // the links, is linear time. The other way requires
  // searching for rightmost, and would be O(n log n).
  // So free the tree first, then the (now empty) list.
  free_tree(os->map);
  free_list(os->order);
  free(os);
}

int main(void)
{
  ordered_strings *os = new_ordered_strings();

  add_string(os, "foo");
  add_string(os, "bar");
  add_string(os, "baz");
  add_string(os, "qux");
  add_string(os, "qax");

  print_list(os->order);
  print_tree(os->map); printf("\n\n");

  printf("removing 'bar'\n");
  remove_string(os, "bar");
  print_list(os->order);
  print_tree(os->map); printf("\n\n");

  printf("Removing index 1 (baz)\n");
  remove_index(os, 1); // baz
  print_list(os->order);
  print_tree(os->map); printf("\n\n");

  printf("Removing index -3 (foo)\n");
  remove_index(os, -3);
  print_list(os->order);
  print_tree(os->map); printf("\n\n");

  printf("all done\n");
  free_ordered_strings(os);

  return 0;
}
