#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define size_check(n,type)                 \
  ((SIZE_MAX / sizeof(type)) >= (n))

#define checked_malloc(n,type)             \
  (size_check((n),(type)) ?                \
    malloc((n) * sizeof(type)) : 0)

#define checked_realloc(p,n,type)          \
  (size_check((n),(type)) ?                \
    realloc((p), (n) * sizeof(type)) : 0)

#define max_array_len(type)               \
  (SIZE_MAX / sizeof(type))

#define at_max_len(n,type)                \
  ((n) == max_array_len(type))

#define capped_dbl(n,type)                \
  (((n) < max_array_len(type) / 2)        \
    ? (2 * (n)) : max_array_len(type))

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN_ARRAY_SIZE 1


struct dynarray {
  size_t size;
  size_t used;
  // Update int * => void **
  void **data;
};

#define da_at(da,i) (da)->data[(i)]
#define da_len(da)  (da)->used

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
  // Updated int * => void **
  void **new_data = checked_realloc(da->data, alloc_size, *da->data);
  if (!new_data) return false;

  da->data = new_data;
  da->size = alloc_size;
  da->used = MIN(da->used, new_size);
  return true; // success
}

// Update val type int => void *
bool da_append(struct dynarray *da, void *val)
{
  if (da->used == da->size) {
    if (at_max_len(da->size, *da->data)) return false;
    size_t new_size = capped_dbl(da->size, *da->data);
    int resize_success = da_resize(da, new_size);
    if (!resize_success) return false;
  }
  da->data[da->used++] = val;
  return true;
}




int main(void)
{
  struct dynarray da;

  int success = da_init(&da, 0, 0);
  if (!success) {
    printf("allocation error\n");
  }

#if 0
  // Can't use values...
  da_append(&da, 42);
  // Can't take addresses of values
  da_append(&da, &42);
#endif

  // We need a variable
  int i = 13;
  da_append(&da, &i);
  // Compound literal, a stack variable.
  da_append(&da, &(int){42});

  for (int i = 0; i < 5; ++i) {
    da_append(&da, &i);
  }
  for (int i = 10; i < 15; ++i) {
    printf("%p %d\n", (void *)&i, i);
  }

  // not pretty...
  for (int i = 0; i < da_len(&da); i++) {
    printf("%d ", *(int *)da_at(&da, i));
  }
  printf("\n");

  for (int i = 0; i < 5; ++i) {
    // These are all the same local integer!
    // We are filling the array with five
    // copies of 4.
    da_append(&da, &(int){i});
  }
  printf("%p\n", da.data[da.used - 1]);
  for (int i = 0; i < 5; ++i) {
    // These are all the same local integer!
    // We are filling the array with five
    // copies of 4.
    da_append(&da, &(int){i});
  }
  printf("%p\n", da.data[da.used - 1]);


  // not pretty...
  for (int i = 0; i < da_len(&da); i++) {
    printf("%d ", *(int *)da_at(&da, i));
  }
  printf("\n");

  // not prettier
#define da_get_deref(da,i,type) *(type *)(da_at((da),(i)))
  for (int i = 0; i < da_len(&da); i++) {
    printf("%d ", da_get_deref(&da, i, int));
  }
  printf("\n");

  // need this to avoid freeing stack objects
  da_resize(&da, 0);
  // if we didn't get rid of the stack objects
  // above, then freeing the integers below
  // would be a problem

  for (int i = 0; i < 5; ++i) {
    int *p = malloc(sizeof *p);
    if (!p) continue;
    *p = i;
    da_append(&da, p);
  }

  for (int i = 0; i < da_len(&da); i++) {
    printf("%d ", *(int*)da_at(&da, i));
  }
  printf("\n");

  for (int i = 0; i < da_len(&da); i++) {
    free(da_at(&da, i));
  }

  da_resize(&da, 1);
  da_at(&da, 0) = &(double){3.14};
  int *p = da_at(&da, 0);
  printf("This is probably not a good idea! %d\n", *p);

  da_dealloc(&da);

  return 0;
}
