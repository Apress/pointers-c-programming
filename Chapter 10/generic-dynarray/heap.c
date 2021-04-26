#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) ((a) < (b)) ? (a) : (b)


struct da_meta {
  size_t size;
  size_t used;
};

#define dynarr(TYPE)                   \
struct {                               \
  struct da_meta meta;                 \
  TYPE   data[];                       \
}

// Always free if we cannot reallocate
void *realloc_dynarray_mem(struct da_meta *p,
                           size_t meta_size,
                           size_t obj_size,
                           size_t new_len)
{
  // Is there a size overflow?
  if (((SIZE_MAX - meta_size) / obj_size < new_len))
    goto fail;

  struct da_meta *new_da =
    realloc(p, meta_size + obj_size * new_len);
  if (!new_da) goto fail;

  new_da->size = new_len;
  new_da->used = MIN(new_da->used, new_len);

  return new_da;

fail:
  free(p);
  return 0;
}

void *new_dynarray_mem(size_t meta_size,
                       size_t obj_size,
                       size_t len)
{
  struct da_meta *array =
    realloc_dynarray_mem(0, meta_size, obj_size, len);
  if (array) array->used = 0;
  return array;
}

void *grow_dynarray_mem(struct da_meta *p,
                        size_t meta_size,
                        size_t obj_size)
{
  // Can we double the length?
  size_t used = meta_size + obj_size * p->size;
  size_t adding = MAX(1, p->size);
  if ((SIZE_MAX - used) / obj_size < adding) {
    free(p);
    return 0;
  }

  return realloc_dynarray_mem(
    p, meta_size, obj_size, p->size + adding
  );
}

#define da_data_offset(da)                       \
  ((char *)&(da)->data - (char *)(da))

#define new_da(da, init_size)                    \
  ((da) = 0, new_dynarray_mem(                   \
                   da_data_offset(da),           \
                   sizeof *(da)->data,           \
                   (init_size)))

#define da_free(da)                              \
  do { free(da); (da) = 0; } while(0)

#define da_at(da,i)  (da->data[(i)])
#define da_len(da)   (da->meta.used)

#define da_append(da, ...)                       \
do {                                             \
  if ((da)->meta.used == (da)->meta.size) {      \
    (da) = grow_dynarray_mem(                    \
      (struct da_meta *)(da),                    \
      da_data_offset(da), sizeof *(da)->data     \
    );                                           \
    if (!(da)) break;                            \
  }                                              \
  (da)->data[(da)->meta.used++] = __VA_ARGS__;   \
} while (0)


int main(void)
{
  dynarr(int) *int_array = new_da(int_array, 0);
  if (!int_array) goto error;
  printf("%zu out of %zu\n",
         int_array->meta.used,
         int_array->meta.size);

  for (int i = 0; i < 5; i++) {
    da_append(int_array, i);
    if (!int_array) goto error;
  }

  for (int i = 0; i < da_len(int_array); i++) {
    printf("%d ", da_at(int_array, i));
  }
  printf("\n");

  da_free(int_array);

  return 0;

error:
  return 1;
}
