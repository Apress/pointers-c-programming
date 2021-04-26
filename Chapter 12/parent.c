#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef struct node *stree;
struct node {
  int value;
  struct node *parent;
  struct node *left;
  struct node *right;
};

int allocated;
stree node(int value, stree parent,
           stree left, stree right)
{
  allocated++;
  stree t = malloc(sizeof *t);
  if (t) *t = (struct node){
    .value = value, .parent = parent,
    .left = left, .right = right
  };
  return t;
}
#define leaf(V,P) node(V, P, 0, 0)

stree *new_stree(void)
{
  stree *t = malloc(sizeof *t);
  if (t) *t = 0;
  return t;
}

void free_nodes(struct node *n);

#define clear_stree(t) \
  do { free_nodes(*t); *t = 0; } while(0)

#define free_stree(t) \
  do { free_nodes(*t); free(t); t = 0; } while(0)


// Find parent and child
stree *find_loc(stree *t, int val, stree *p)
{
  *p = 0; // root parent is NULL
  while (*t && (*t)->value != val) {
    *p = *t;
    if (val < (*t)->value) t = &(*t)->left;
    else                   t = &(*t)->right;
  }
  return t;
}

bool contains(stree *t, int val)
{
  stree parent = 0;
  return !! *find_loc(t, val, &parent);
}

bool insert(stree *t, int val)
{
  stree parent = 0;
  stree *target = find_loc(t, val, &parent);
  if (*target) return true; // already there
  else return !!(*target = leaf(val, parent));
}


stree *rightmost(stree *t, stree *p)
{
  while ((*t)->right) {
    *p = *t;
    t = &(*t)->right;
  }
  return t;
}

void delete(stree *t, int val)
{
  stree parent = 0;
  stree *loc = find_loc(t, val, &parent);

  if (*loc) {
    stree t = *loc;
    if (!(t->left && t->right)) {
      *loc = t->left ? t->left : t->right;
      if (*loc) (*loc)->parent = parent;
      free(t); allocated--;

    } else {
      parent = t;
      stree *rm_ref = rightmost(&t->left, &parent);
      stree rm = *rm_ref;
      t->value = rm->value;
      *rm_ref = rm->left;
      if (*rm_ref) (*rm_ref)->parent = parent;
      free(rm); allocated--;
    }
  }
}

stree *make_stree(int n, int array[n])
{
  stree *t = new_stree();
  if (!t) return 0;
  for (int i = 0; i < n; i++) {
    if (!insert(t, array[i])) {
      free_stree(t);
      return 0;
    }
  }
  return t;
}

#if 1

void print_stree(stree *t)
{
  if (!*t) return;
  putchar('(');
    print_stree(&(*t)->left);
    putchar(',');
    printf("%d", (*t)->value);
    putchar(',');
    print_stree(&(*t)->right);
  putchar(')');
}

#else

#define indent(I) do { for (int i = 0; i < I; i++) putchar(' '); } while(0)
void print_stree_indent(stree *t, int indent)
{
  if (!*t) return;
  indent(indent); printf("[%d] <%p>\n", (*t)->value, (void*)*t);
  print_stree_indent(&(*t)->left, indent + 2);
  print_stree_indent(&(*t)->right, indent + 2);
}
#define print_stree(t) print_stree_indent(t, 0)
#endif

#define left_child(t) \
  ((t)->parent && (t)->parent->left == (t))
void parent_traverse(stree t)
{
  enum { DOWN, UP } state = DOWN;
  while (t) {
    switch (state) {
      case DOWN:
        // Recurse as far left as we can...
        while (t->left) { putchar('('); t = t->left; }
        // Emit the leaf we find there
        printf("(,%d,", t->value); // VISIT
        // Then go right, if we can, or up if we can't.
        if (t->right) { t = t->right; }
        else          { putchar(')'); state = UP; }
        break;

      case UP:
        if (!t->parent) return; // we have returned to the root...
        if (left_child(t)) {
          // Returning from a left child, we emit the parent
          t = t->parent;
          printf(",%d,", t->value); // VISIT
          // Then we go right if we can't, or continue up
          // (t is already the parent) if we cannot.
          if (t->right) { t = t->right; state = DOWN; }
          else          { putchar(')'); }
        } else {
          // Returning from a right child just means going up
          putchar(')'); t = t->parent;
        }
        break;
    }
  }
}

void check_tree(stree t)
{
  if (!t) return;
  if (t->left)  assert(t->left->parent == t);
  if (t->right) assert(t->right->parent == t);
  check_tree(t->left); check_tree(t->right);
}

void free_nodes(struct node *n)
{
  stree parent; // just for rightmost. FIXME: rewrite function
  struct node *curr = n;
  while (curr) {
    if (!curr->left) {
      struct node *right = curr->right;
      free(curr); allocated--;
      curr = right;
    } else {
      // Since we delete the left links, left will be NULL
      // when we return to node, so we never find curr again
      (*rightmost(&curr->left, &parent))->right = curr;
      // Remove the left link so we don't go down that tree
      // again...
      struct node *left = curr->left;
      curr->left = 0;
      curr = left;
    }
  }
}

void parent_free(stree t)
{
  struct node *p;
  enum { DOWN, UP } state = DOWN;
  while (t) {
    switch (state) {
      case DOWN:
        while (t->left) { t = t->left; }
        if (t->right) { t = t->right; }
        else          { state = UP; }
        break;

      case UP:
        if (!t->parent) { free(t); allocated--; return; }
        if (left_child(t)) {
          p = t->parent; free(t); allocated--; t = p;
          if (t->right) { t = t->right; state = DOWN; }
        } else {
          p = t->parent; free(t); allocated--; t = p;
        }
        break;
    }
  }
}

#define free_nodes parent_free


int main(void)
{
  stree t = 0;
  insert(&t, 2);
  insert(&t, 3);
  insert(&t, 1);
  insert(&t, 6);

  print_stree(&t); putchar('\n');
  check_tree(t);
  parent_traverse(t); putchar('\n');

  assert(contains(&t, 2));
  assert(contains(&t, 1));
  assert(contains(&t, 3));
  assert(!contains(&t, 10));
  assert(!contains(&t, 0));

  insert(&t, 10);
  insert(&t, 0);
  check_tree(t);

  assert(contains(&t, 10));
  assert(contains(&t, 0));


  print_stree(&t);    putchar('\n');
  parent_traverse(t); putchar('\n');
  check_tree(t);

  printf("deleting\n");
  delete(&t, 12);
  delete(&t, 3);
  delete(&t, 6);

  assert(!contains(&t, 3));
  assert(!contains(&t, 6));

  print_stree(&t);    putchar('\n');
  parent_traverse(t); putchar('\n');
  check_tree(t);
  clear_stree(&t);

  int array[] = { 1, 2, 13, 4, 16, 8, 10 };
  int n = sizeof array / sizeof *array;
  stree *t2 = make_stree(n, array);
  print_stree(t2); putchar('\n');
  parent_traverse(*t2); putchar('\n');
  free_stree(t2);



  printf("allocated == %d\n", allocated);

  return 0;
}
