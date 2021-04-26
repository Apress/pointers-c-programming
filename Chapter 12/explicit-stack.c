#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <setjmp.h>

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

struct free_frame {
  struct node       *node;
  struct free_frame *next;
};

void free_push(struct free_frame **stack, struct node *node)
{
  if (!node) return;
  struct free_frame *frame = malloc(sizeof *frame);
  if (!frame) abort(); // We do not tolerate errors!
  *frame = (struct free_frame){ .node = node, .next = *stack };
  *stack = frame;
}

struct node *free_pop(struct free_frame **stack)
{
  struct free_frame *top = *stack;
  struct node *node = top->node;
  *stack = top->next;
  free(top);
  return node;
}

void free_nodes(struct node *n)
{
  struct free_frame *stack = 0;
  free_push(&stack, n);
  while (stack) {
    n = free_pop(&stack);
    free_push(&stack, n->left);
    free_push(&stack, n->right);
    free(n);
  }
}

enum print_op {
  LPAR, TREE, COMMA, VALUE, RPAR
};
struct print_frame {
  enum print_op op;
  struct node *node;
  struct print_frame *next;
};
struct print_stack {
  struct print_frame *frames;
  jmp_buf env;
};

void print_push(struct print_stack *stack,
                enum print_op op, struct node *node)
{
  struct print_frame *frame = malloc(sizeof *frame);
  if (!frame) longjmp(stack->env, 1); // bail!
  *frame = (struct print_frame){
    .op = op, .node = node, .next = stack->frames
  };
  stack->frames = frame;
}

void schedule_print(struct print_stack *stack,
                    struct node *node)
{
  print_push(stack, RPAR, 0);
  if (node->right) print_push(stack, TREE, node->right);
  print_push(stack, COMMA, 0);
  print_push(stack, VALUE, node);
  print_push(stack, COMMA, 0);
  if (node->left) print_push(stack, TREE, node->left);
  print_push(stack, LPAR, 0);
}

void handle_print_op(enum print_op op, struct node *node,
                     struct print_stack *stack)
{
  switch (op) {
    case LPAR:  putchar('('); break;
    case RPAR:  putchar(')'); break;
    case COMMA: putchar(','); break;
    case VALUE: printf("%d", node->value);   break;
    case TREE:  schedule_print(stack, node); break;
  }
}

void print_stree(stree *t)
{
  if (!*t) return;

  enum print_op op;
  struct node *n = 0;
  struct print_stack stack = { .frames = 0 };
  if (setjmp(stack.env) != 0) goto error;

  schedule_print(&stack, *t);
  while (stack.frames) {
    struct print_frame *top = stack.frames;
    op = top->op; n = top->node;
    stack.frames = top->next;
    free(top);

    handle_print_op(op, n, &stack);
  }
  return;

error:
  // Cleanup stack memory
  while (stack.frames) {
    struct print_frame *top = stack.frames;
    stack.frames = top->next;
    free(top);
  }
  // We cannot undo the damage we have done to the output, though.
  // We have written parts of the tree, and then bailed...
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
