#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef struct node *stree;
struct node {
  int value;
  struct node *left;
  struct node *right;
};

stree node(int value, stree left, stree right)
{
  stree t = malloc(sizeof *t);
  if (t) *t = (struct node){
    .value = value, .left = left, .right = right
  };
  return t;
}
#define leaf(V) node(V, 0, 0)


void print_stree(stree *t);

stree *new_stree(void)
{
  stree *t = malloc(sizeof *t);
  if (t) *t = 0;
  return t;
}

// Not tail recursive
void free_nodes(struct node *n)
{
  if (!n) return;
  free_nodes(n->left);
  free_nodes(n->right);
  free(n);
}

#define clear_stree(t) \
  do { free_nodes(*t); *t = 0; } while(0)

#define free_stree(t) \
  do { free_nodes(*t); free(t); t = 0; } while(0)

// tail recursive
bool contains(stree *tp, int val)
{
  assert(tp);
  stree t = *tp;
  if (!t)              return false;
  if (val == t->value) return true;
  if (val < t->value)  return contains(&t->left, val);
  else                 return contains(&t->right, val);
}

// Tail recursive, and we handle errors
bool insert(stree *target, int val)
{
  assert(target);
  stree t = *target;
  if (!t) {
    // we return the result of the allocation,
    // which means we return true the allocation
    // succeeded
    return !!(*target = leaf(val));
  }
  if (val < t->value) {
    return insert(&t->left, val);
  } else {
    return insert(&t->right, val);
  }
}

// tail recursive
stree *rightmost(stree *t)
{
  assert(t && *t);
  if (!(*t)->right) return t;
  else return rightmost(&(*t)->right);
}

// Tail recursive
void delete(stree *target, int val)
{
  assert(target);
  stree t = *target;

  if (!t) return;

  if (val == t->value) {
    if (!(t->left && t->right)) {
      stree subtree = t->left ? t->left : t->right;
      *target = subtree;
      free(t);
    } else {
      stree *rm_ref = rightmost(&t->left);
      stree rm = *rm_ref;
      t->value = rm->value;
      *rm_ref = rm->left;
      free(rm);
    }

  } else if (val < t->value) {
    delete(&t->left, val);
  } else if (val > t->value) {
    delete(&t->right, val);
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

// Not tail recursive
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


int main(void)
{
#if 0
  stree t =
    node(3,
      node(2, leaf(1), 0),
      leaf(6));
#endif
  stree t = 0;
  insert(&t, 3);
  insert(&t, 2);
  insert(&t, 1);
  insert(&t, 6);

  print_stree(&t); putchar('\n');

  assert(contains(&t, 2));
  assert(contains(&t, 1));
  assert(contains(&t, 3));
  assert(!contains(&t, 10));
  assert(!contains(&t, 0));

  insert(&t, 10);
  insert(&t, 0);

  assert(contains(&t, 10));
  assert(contains(&t, 0));


  print_stree(&t); putchar('\n');

  delete(&t, 12);
  printf("deleting 12:\n"); print_stree(&t); putchar('\n');
  delete(&t, 3);
  printf("deleting 3:\n"); print_stree(&t); putchar('\n');
  delete(&t, 6);
  printf("deleting 6:\n"); print_stree(&t); putchar('\n');

  assert(!contains(&t, 3));
  assert(!contains(&t, 6));

  print_stree(&t); putchar('\n');
  clear_stree(&t);

  int array[] = { 1, 2, 3, 4, 6, 8, 10 };
  int n = sizeof array / sizeof *array;
  stree *t2 = make_stree(n, array);
  print_stree(t2); putchar('\n');
  free_stree(t2);

  return 0;
}
