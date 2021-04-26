#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>   // for max_align_t
#include "refcount.h"

struct refcount {
  // We either have a reference count or we are deleting
  // using a stack.
  union { size_t rc; void *stack; };
  // Callback function for user-defined objects
  void (*cleanup)(void *, void *);
  max_align_t user_data[];
};

#define container(p,type,member)                  \
    (type *)((char *)p - offsetof(type, member))
#define refcount_mem(p)                           \
    container(p, struct refcount, user_data)
#define user_mem(rc)                              \
    (void *)(rc->user_data)

#define RCSIZE offsetof(struct refcount, user_data)
void *rc_alloc(size_t size, void (*cleanup)(void *, void *))
{
  struct refcount *rc = malloc(RCSIZE + size);
  if (!rc) return 0;

  rc->rc = 1;
  rc->cleanup = cleanup;

  return user_mem(rc);
}

void *incref(void *p)
{
  if (!p) return p; // it makes some code easier if we allow this
  struct refcount *rc = refcount_mem(p);
  rc->rc++;
  return p;
}

void cleanup(struct refcount *stack)
{
  while (stack) {
    if (stack->cleanup)
      stack->cleanup(user_mem(stack), stack);
    struct refcount *next = stack->stack;
    free(stack);
    stack = next;
  }
}

void *decref_ctx(void *p, void *ctx)
{
  if (!p) return p; // accept NULL as free() would...

  struct refcount *rc = refcount_mem(p);
  if (--rc->rc == 0) {
    rc->stack = 0; // probably already is, with rc == 0, but still...
    if (ctx) {
      // Schedule for deletion
      struct refcount *stack = ctx;
      rc->stack = stack->stack;
      stack->stack = rc;
    } else {
      // Start cleanup
      cleanup(rc);
    }
    return 0; // reference is now gone...
  }

  return p;
}
