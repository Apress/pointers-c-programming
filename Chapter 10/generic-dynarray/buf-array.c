#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define size_check(n,obj_size)                 \
  ((SIZE_MAX / (obj_size)) >= (n))

// Use object size instead of type
#define checked_malloc(n,obj_size)             \
  (size_check((n),(obj_size)) ?                \
    malloc((n) * (obj_size)) : 0)

#define checked_realloc(p,n,obj_size)          \
  (size_check((n),(obj_size)) ?                \
    realloc((p), (n) * (obj_size)) : 0)

#define max_array_len(obj_size)               \
  (SIZE_MAX / obj_size)

#define at_max_len(n,obj_size)                \
  ((n) == max_array_len(obj_size))

#define capped_dbl(n,obj_size)                \
  (((n) < max_array_len(obj_size) / 2)        \
    ? (2 * (n)) : max_array_len(obj_size))

#define MIN(a,b) ((a) < (b)) ? (a) : (b)
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN_ARRAY_SIZE 1


struct dynarray {
  size_t size;
  size_t used;
  // Added size of objects
  size_t obj_size;
  // Update int * => char *
  char *data;
};

// We need separate getters and setters.

// Compute the address instead of index into array
#define da_at(da,i) (void *)((da)->data + (i) * (da)->obj_size)
#define da_at_as(da,i,type) *(type *)da_at(da,i)

#define da_len(da) (da)->used

bool da_init(struct dynarray *da,
             size_t init_size,
             size_t init_used,
             size_t obj_size)
{
  assert(init_size >= init_used);
  init_size = MAX(init_size, MIN_ARRAY_SIZE);
  da->data = checked_malloc(init_size, obj_size);
  da->size = (da->data) ? init_size : 0;
  da->used = (da->data) ? init_used : 0;
  da->obj_size = obj_size;
  return !!da->data;
}

void da_dealloc(struct dynarray *da)
{
  free(da->data);
  da->data = 0;
  da->size = da->used = 0;
}

bool da_resize(struct dynarray *da,
               size_t new_size)
{
  size_t alloc_size = MAX(new_size, MIN_ARRAY_SIZE);
  // Updated int * => char *
  char *new_data =
    checked_realloc(da->data, alloc_size, da->obj_size);
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
    if (at_max_len(da->size, da->obj_size)) return false;
    size_t new_size = capped_dbl(da->size, da->obj_size);
    int resize_success = da_resize(da, new_size);
    if (!resize_success) return false;
  }
  // copy memory...
  memcpy(da->data + da->used * da->obj_size,
         val, da->obj_size);
  da->used++;
  return true;
}



int main(void)
{
  struct dynarray da;

  // include sizeof here
  int success = da_init(&da, 0, 0, sizeof(int));
  if (!success) {
    printf("allocation error\n");
  }

  for (int i = 0; i < 5; ++i) {
    da_append(&da, &i);
  }

  for (int i = 0; i < 5; ++i) {
    da_at_as(&da, i, int) += 10;
  }

  for (int i = 0; i < da_len(&da); i++) {
    printf("%d ", da_at_as(&da, i, int));
  }
  printf("\n");


  da_dealloc(&da);

  return 0;
}
