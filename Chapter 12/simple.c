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

// tail recursive
bool contains(stree t, int val)
{
  if (!t)              return false;
  if (val == t->value) return true;
  if (val < t->value)  return contains(t->left, val);
  else                 return contains(t->right, val);
}

#if 0
// Not tail recursive, and we don't handle allocation errors
stree insert(stree t, int val)
{
  if (!t) return leaf(val); // can fail, but we don't handle it
  if (val < t->value) {
    t->left = insert(t->left, val);
  } else {
    t->right = insert(t->right, val);
  }
  return t;
}
#else
// Not tail recursive
stree insert_node(stree t, struct node *n)
{
  if (!t) return n;
  if (n->value == t->value) {
    free(n); // it was already here
  } else if (n->value < t->value) {
    t->left = insert_node(t->left, n);
  } else {
    t->right = insert_node(t->right, n);
  }
  return t;
}

stree insert(stree t, int val)
{
  struct node *n = leaf(val);
  if (!n) return 0;
  return insert_node(t, n);
}

#endif
// tail recursive
int rightmost_val(stree t)
{
  assert(t);
  if (!t->right) return t->value;
  else return rightmost_val(t->right);
}

// not tail recursive
stree delete(stree t, int val)
{
  if (!t) return t;

  if (val == t->value) {
    if (!(t->left && t->right)) {
      stree subtree = t->left ? t->left : t->right;
      free(t);
      return subtree;
    } else {
      t->value = rightmost_val(t->left);
      t->left = delete(t->left, t->value);
    }

  } else if (val < t->value) {
    t->left = delete(t->left, val);
  } else if (val > t->value) {
    t->right = delete(t->right, val);
  }

  return t;
}

stree make_stree(int n, int array[n])
{
  stree t = 0;
  for (int i = 0; i < n; i++) {
    t = insert(t, array[i]);
  }
  return t;
}

// Not tail recursive
void print_stree(stree t)
{
  if (!t) return;
  putchar('(');
    print_stree(t->left);
    putchar(',');
    printf("%d", t->value);
    putchar(',');
    print_stree(t->right);
  putchar(')');
}

// Not tail recursive
void free_stree(stree t)
{
  if (!t) return;
  free_stree(t->left);
  free_stree(t->right);
  free(t);
}


int main(void)
{
  stree t =
    node(3,
      node(2, leaf(1), 0),
      leaf(6));

  t = insert(t, 10);
  t = insert(t, 0);


  print_stree(t); putchar('\n');

  t = delete(t, 12);
  t = delete(t, 3);

  print_stree(t); putchar('\n');
  free_stree(t);

  int array[] = { 1, 2, 3, 4, 6, 8, 10 };
  int n = sizeof array / sizeof *array;
  t = make_stree(n, array);
  print_stree(t); putchar('\n');
  free_stree(t);

  return 0;
}
