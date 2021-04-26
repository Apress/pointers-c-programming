#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef struct node *stree;
struct node {
  int value;
  struct node *left;
  struct node *right;
  struct node *stack;
};

stree node(int value, stree left, stree right)
{
  stree t = malloc(sizeof *t);
  if (t) *t = (struct node){
    .value = value,
    .left = left, .right = right,
    .stack = 0
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

// tail recursive
stree *rightmost(stree *t)
{
  assert(t && *t);
  if (!(*t)->right) return t;
  else return rightmost(&(*t)->right);
}

void delete(stree *t, int val)
{
  stree *target = find_loc(t, val);
  if (*target) {
    stree t = *target;
    if (!(t->left && t->right)) {
      *target = t->left ? t->left : t->right;
      free(t);
    } else {
      stree *rm_ref = rightmost(&t->left);
      stree rm = *rm_ref;
      t->value = rm->value;
      *rm_ref = rm->left;
      free(rm);
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

void push_node(struct node **stack,
               struct node *node)
{
  if (node) {
    node->stack = *stack;
    *stack = node;
  }
}

struct node *pop_node(struct node **stack)
{
  struct node *top = *stack;
  *stack = top->stack;
  top->stack = 0;
  return top;
}

void print_stack(struct node *stack)
{
  printf("stack: [");
  while (stack) {
    printf("%d, ", stack->value);
    stack = stack->stack;
  }
  printf("]\n");
}

void free_nodes(struct node *n)
{
  struct node *stack = 0;
  push_node(&stack, n);
  while (stack) {
    n = pop_node(&stack);
    push_node(&stack, n->left);
    push_node(&stack, n->right);
    free(n);
  }
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
