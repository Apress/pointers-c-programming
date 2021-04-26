#include <stdlib.h>
#include <assert.h>


struct node {
  struct node *left, *right;
  int val;
};

struct node_pool {
  size_t size, used;
  struct node *pool;
};

struct node_pool *new_pool(size_t capacity)
{
  struct node_pool *pool = malloc(sizeof *pool);
  if (!pool) return 0;
  pool->pool = malloc(capacity * sizeof *pool->pool);
  if (!pool->pool) { free(pool); return 0; }
  pool->size = capacity;
  pool->used = 0;
  return pool;
}

struct node *node_alloc(struct node_pool *pool)
{
  if (pool->used == pool->size) return 0; // Pool is used up
  return &pool->pool[pool->used++];
}

void free_pool(struct node_pool *pool)
{
  free(pool->pool);
  free(pool);
}

int main(void)
{
  struct node_pool *pool = new_pool(3);
  struct node *n1 = node_alloc(pool);
  struct node *n2 = node_alloc(pool);
  struct node *n3 = node_alloc(pool);
  struct node *n4 = node_alloc(pool);
  assert(n2 - n1 == 1);
  assert(n3 - n2 == 1);
  assert(n4 == 0);
  return 0;
}
