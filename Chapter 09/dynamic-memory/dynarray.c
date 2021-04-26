#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

struct dynarray {
  size_t size;
  size_t used;
  int *data;
};

#define da_at(da,i)     (da)->data[(i)]
#define da_len(da)      (da)->used

#define size_check(n,type)                 \
  ((SIZE_MAX / sizeof(type)) >= (n))
#define checked_malloc(n,type)             \
  (size_check((n),(type)) ?                \
    malloc((n) * sizeof(type)) : 0)
#define checked_realloc(p,n,type)          \
  (size_check((n),(type)) ?                \
    realloc((p), (n) * sizeof(type)) : 0)

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN_ARRAY_SIZE 1

bool da_init(struct dynarray *da,
             size_t init_size,
             size_t init_used)
{
  assert(init_size >= init_used);
  init_size = MAX(init_size, MIN_ARRAY_SIZE);
  da->data = checked_malloc(init_size, *da->data);
  da->size = (da->data) ? init_size : 0;
  da->used = (da->data) ? init_used : 0;
  return !!da->data;
}

void da_dealloc(struct dynarray *da)
{
  free(da->data);
  da->data = 0;
  da->size = da->used = 0;
}

#define MIN(a,b) ((a) < (b)) ? (a) : (b)
bool da_resize(struct dynarray *da,
               size_t new_size)
{
  size_t alloc_size = MAX(new_size, MIN_ARRAY_SIZE);
  int *new_data = checked_realloc(da->data, alloc_size, *da->data);
  // If we cannot allocate, leave everything
  // as it is, but report an error
  if (!new_data) return false;

  da->data = new_data;
  da->size = alloc_size;
  da->used = MIN(da->used, new_size);
  return true; // success
}

#define max_array_len(type)               \
  (SIZE_MAX / sizeof(type))

#define at_max_len(n,type)                \
  ((n) == max_array_len(type))

#define capped_dbl(n,type)                \
  (((n) < max_array_len(type) / 2)        \
    ? (2 * (n)) : max_array_len(type))

bool da_append(struct dynarray *da, int val)
{
  if (da->used == da->size) {
    if (at_max_len(da->size, *da->data)) return false;
    size_t new_size = capped_dbl(da->size, *da->data);
    new_size = (new_size > 0) ? new_size : 1;
    int resize_success = da_resize(da, new_size);
    if (!resize_success) return false;
  }
  da->data[da->used++] = val;
  return true; // success
}

int main(void)
{
  struct dynarray da;

  int success = da_init(&da, 4, 4);
  if (!success) {
    printf("allocation error\n");
  }

  for (int i = 0; i < da_len(&da); i++) {
    da_at(&da, i) = i;
  }
  for (int i = 0; i < da_len(&da); i++) {
    printf("%d ", da_at(&da, i));
  }
  printf("\n");

  printf("current length %zu\n", da_len(&da));
  for (int i = 0; i < 10; i++) {
    if (!da_append(&da, 10 * (i + 1))) {
      printf("allocation error");
      break;
    }
  }
  printf("current length %zu\n", da_len(&da));
  for (int i = 0; i < da_len(&da); i++) {
    printf("%d ", da_at(&da, i));
  }
  printf("\n");

  da_dealloc(&da);

  return 0;
}
