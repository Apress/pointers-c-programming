#include <stdio.h>
#include <assert.h>
#include "list.h"
#include "dynarray.h"

typedef void *  impl_stack;
typedef int     elem;

typedef struct {
  impl_stack (*new_stack)   (void);
  bool       (*empty_stack) (impl_stack);
  elem       (*top)         (impl_stack);
  bool       (*push)        (impl_stack, elem);
  elem       (*pop)         (impl_stack);
  void       (*free_stack)  (impl_stack);
} stack_type;

struct stack {
  void       *impl_stack;
  stack_type *type;
};
typedef struct stack * stack;

stack new_stack(stack_type *type)
{
  void *impl_stack = type->new_stack();
  stack stack = malloc(sizeof *stack);
  if (!impl_stack || !stack) goto error;

  stack->impl_stack = impl_stack;
  stack->type = type;
  return stack;

error:
  free(stack);
  if (impl_stack)
    type->free_stack(impl_stack);
  return 0;
}

bool empty_stack(stack stack)
{
  return stack->type->empty_stack(stack->impl_stack);
}
elem stack_top(stack stack)
{
  return stack->type->top(stack->impl_stack);
}
bool stack_push(stack stack, elem elem)
{
  return stack->type->push(stack->impl_stack, elem);
}
elem stack_pop(stack stack)
{
  return stack->type->pop(stack->impl_stack);
}
void free_stack(stack stack)
{
  stack->type->free_stack(stack->impl_stack);
  free(stack);
}

// List stack
impl_stack list_stack_new(void)
{
  return new_list();
}
bool list_stack_empty(impl_stack stack)
{
  return is_empty((list)stack);
}
elem list_stack_top(impl_stack stack)
{
  return front((list)stack)->value;
}
bool list_stack_push(impl_stack stack, elem elem)
{
  return prepend(stack, elem);
}
elem list_stack_pop(impl_stack stack)
{
  elem elem = front((list)stack)->value;
  delete_link(front((list)stack));
  return elem;
}
void list_stack_free(impl_stack stack)
{
  free_list(stack);
}

stack_type list_stack = {
  .new_stack   = list_stack_new,
  .empty_stack = list_stack_empty,
  .top         = list_stack_top,
  .push        = list_stack_push,
  .pop         = list_stack_pop,
  .free_stack  = list_stack_free
};

// Dynarray stack
impl_stack da_stack_new(void)
{
  struct dynarray *da = malloc(sizeof *da);
  if (!da) return 0;
  if (!da_init(da, 1, 0)) {
    free(da);
    return 0;
  }
  return da;
}
bool da_stack_empty(impl_stack stack)
{
  return ((struct dynarray *)stack)->used == 0;
}
elem da_stack_top(impl_stack stack)
{
  struct dynarray *da = stack;
  assert(da->used > 0);
  return da->data[da->used - 1];
}
bool da_stack_push(impl_stack stack, elem elem)
{
  return da_append(stack, elem);
}
elem da_stack_pop(impl_stack stack)
{
  struct dynarray *da = stack;
  assert(da->used > 0);
  return da->data[--(da->used)];
}
void da_stack_free(impl_stack stack)
{
  da_dealloc(stack);
  free(stack);
}

stack_type da_stack = {
  .new_stack   = da_stack_new,
  .empty_stack = da_stack_empty,
  .top         = da_stack_top,
  .push        = da_stack_push,
  .pop         = da_stack_pop,
  .free_stack  = da_stack_free
};


int main(void)
{
  // Try with list stack
  stack stack = new_stack(&list_stack);
  for (int i = 0; i < 5; i++) {
    stack_push(stack, i);
  }
  while (!empty_stack(stack)) {
    int x = stack_pop(stack);
    printf("%d\n", x);
  }
  free_stack(stack);

  // Try with dynamic array
  stack = new_stack(&da_stack);
  for (int i = 0; i < 5; i++) {
    stack_push(stack, i);
  }
  while (!empty_stack(stack)) {
    int x = stack_pop(stack);
    printf("%d\n", x);
  }
  free_stack(stack);

  return 0;
}
