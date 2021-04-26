#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>


struct node {
  struct node *left, *right;
  int val;
};

struct subpool {
  struct subpool *next;
  struct node nodes[];
};

struct subpool *new_subpool(size_t capacity, struct subpool *next)
{
  struct subpool *pool = 0;
  size_t size = offsetof(struct subpool, nodes) +
                (sizeof *pool->nodes) * capacity;
  pool = malloc(size);
  if (pool) pool->next = next;
  return pool;
}

struct node_pool {
  size_t top_capacity, top_used;
  struct subpool *subpools;
};

struct node_pool *new_pool(size_t init_capacity)
{
  struct node_pool *pool = malloc(sizeof *pool);
  if (!pool) return 0;

  struct subpool *subpool = new_subpool(init_capacity, 0);
  if (!subpool) { free(pool); return 0; }

  pool->top_capacity = init_capacity;
  pool->top_used = 0;
  return pool;
}

struct node *node_alloc(struct node_pool *pool)
{
  if (pool->top_used == pool->top_capacity) {
    // We want to resize by adding a pool twice as large as the
    // current largest pool. But first check if we can.
    if (SIZE_MAX / 2 / sizeof(struct node) < pool->top_capacity) return 0;
    size_t new_size = 2 * pool->top_capacity;
    struct subpool *new_top = new_subpool(new_size, pool->subpools);
    if (!new_top) return 0;

    // Success, so add new pool to list
    pool->subpools = new_top;
    pool->top_capacity = new_size;
    pool->top_used = 0;
  }
  return &pool->subpools->nodes[pool->top_used++];
}

void free_pool(struct node_pool *pool)
{
  struct subpool *sp = pool->subpools;
  while (sp) {
    struct subpool *next = sp->next;
    free(sp);
    sp = next;
  }
  free(pool);
}

int main(void)
{
  struct node_pool *pool = new_pool(3);
  struct node *n1 = node_alloc(pool);
  struct node *n2 = node_alloc(pool);
  struct node *n3 = node_alloc(pool);
  assert(n2 - n1 == 1);
  assert(n3 - n2 == 1);
  // Technically they could sit like that, but it is highly
  // unlikely...
  struct node *n4 = node_alloc(pool);
  assert(n4 - n3 != 1);
  struct node *n5 = node_alloc(pool);
  assert(n5 - n4 == 1);
  struct node *n6 = node_alloc(pool);
  assert(n6 - n5 == 1);
  struct node *n7 = node_alloc(pool);
  assert(n7 - n6 == 1);
  struct node *n8 = node_alloc(pool);
  assert(n8 - n7 == 1);
  struct node *n9 = node_alloc(pool);
  assert(n9 - n8 == 1);
  struct node *n10 = node_alloc(pool);
  assert(n10 - n9 != 1);

  free_pool(pool);

  return 0;
}
