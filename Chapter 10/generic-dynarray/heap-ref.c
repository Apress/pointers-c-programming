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


struct point { double x, y; };
typedef dynarr(struct point) point_array;

// Use a pointer to the dynamic array pointer
void add_points(size_t n,
                double xs[n], double ys[n],
                point_array **da)
{
  for (int i = 0; i < n; i++) {
    da_append(*da, (struct point){ .x = xs[i], .y = ys[i] });
    if (!*da) break;
  }
}

int main(void)
{
  point_array *points = new_da(points, 0);

  size_t n = 5;
  double xs[5] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
  double ys[5] = { 4.0, 3.0, 2.0, 1.0, 0.0 };
  // call by reference
  add_points(n, xs, ys, &points);
  if (!points) goto error;

  for (int i = 0; i < da_len(points); i++) {
    printf("<%.1f,%.1f> ",
           da_at(points, i).x,
           da_at(points, i).y);
  }
  printf("\n");

  da_free(points);
  return 0;

error:
  return 1;
}
