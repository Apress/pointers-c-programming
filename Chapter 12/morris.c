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

int allocated;
stree node(int value, stree left, stree right)
{
  allocated++;
  stree t = malloc(sizeof *t);
  if (t) *t = (struct node){
    .value = value, .left = left, .right = right
  };
  return t;
}
#define leaf(V) node(V, 0, 0)

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


// tail recursive
stree *find_loc(stree *t, int val)
{
  assert(t);
  if (!*t || (*t)->value == val)
    return t;
  else if (val < (*t)->value)
    return find_loc(&(*t)->left, val);
  else
    return find_loc(&(*t)->right, val);
}

bool contains(stree *t, int val)
{
  return !! *find_loc(t, val);
}

bool insert(stree *t, int val)
{
  stree *target = find_loc(t, val);
  if (*target) return true; // already there
  else return !!(*target = leaf(val));
}


stree *rightmost_to(stree *t, struct node *n);
#define rightmost(t) rightmost_to(t, 0)

void delete(stree *t, int val)
{
  stree *target = find_loc(t, val);
  if (*target) {
    stree t = *target;
    if (!(t->left && t->right)) {
      *target = t->left ? t->left : t->right;
      free(t); allocated--;
    } else {
      stree *rm_ref = rightmost(&t->left);
      stree rm = *rm_ref;
      t->value = rm->value;
      *rm_ref = rm->left;
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


stree *rightmost_to(stree *t, struct node *n)
{
  assert(t && *t);
  if ((*t)->right == 0 || (*t)->right == n) {
    return t;
  } else {
    return rightmost_to(&(*t)->right, n);
  }
}

void inorder(struct node *n)
{
  if (!n) return;
  inorder(n->left);
  printf("%d\n", n->value);
  inorder(n->right);
}

void morris(stree *t)
{
  struct node *curr = *t;
  while (curr) {
    if (!curr->left) {
      printf("%d\n", curr->value);
      curr = curr->right;
    } else {
      stree pred = *rightmost_to(&curr->left, curr);
      assert(pred->right == 0 || pred->right == curr);
      if (pred->right == 0) {
        pred->right = curr;
        curr = curr->left;
      } else {
        printf("%d\n", curr->value);
        pred->right = 0;
        curr = curr->right;
      }
    }
  }
}

struct stree_iter {
  struct node *curr;
};

struct stree_iter stree_iter(stree *t)
{
  return (struct stree_iter) { .curr = *t };
}

struct node *next_node(struct stree_iter *iter)
{
  struct node *curr = iter->curr;
  while (curr) {
    if (!curr->left) {
      iter->curr = curr->right;
      return curr; // Yield node

    } else {
      stree pred = *rightmost_to(&curr->left, curr);
      assert(pred->right == 0 || pred->right == curr);

      if (pred->right == 0) {
        pred->right = curr;
        curr = curr->left;
      } else {
        iter->curr = curr->right;
        pred->right = 0;
        return curr; // Yield node
      }
    }
  }

  return 0;
}

void cleanup_iter(struct stree_iter *iter)
{
  // Run to completion to clean up
  while (next_node(iter)) {}
}

void free_nodes(struct node *n)
{
  struct node *curr = n;
  while (curr) {
    if (!curr->left) {
      struct node *right = curr->right;
      free(curr); allocated--;
      curr = right;
    } else {
      // Add pointer so we can go back
      (*rightmost(&curr->left))->right = curr;
      // Recurse left, but make sure we never go left again
      struct node *left = curr->left;
      curr->left = 0;
      curr = left;
    }
  }
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
  delete(&t, 3);
  delete(&t, 6);

  assert(!contains(&t, 3));
  assert(!contains(&t, 6));

  print_stree(&t); putchar('\n');
  clear_stree(&t);

  int array[] = { 1, 2, 13, 4, 16, 8, 10 };
  int n = sizeof array / sizeof *array;
  stree *t2 = make_stree(n, array);
  print_stree(t2); putchar('\n');

  printf("inorder:\n");
  inorder(*t2);
  printf("morris\n");
  morris(t2);

  printf("iterator:\n");
  struct stree_iter iter = stree_iter(t2);
  struct node *node = 0;
  while ( (node = next_node(&iter)) ) {
    printf("%d\n", node->value);
  }
  cleanup_iter(&iter);

  free_stree(t2);

  t2 = make_stree(n, array);
  free_nodes(*t2);
  free(t2);

  printf("allocated == %d\n", allocated);

  return 0;
}
