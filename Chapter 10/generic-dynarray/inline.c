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
#define MIN(a,b) ((a) < (b)) ? (a) : (b)
#define MIN_ARRAY_SIZE 1

#define da_at(da,i)  ((da).data[(i)])
#define da_len(da)   ((da).used)

#define dynarray(TYPE)                 \
struct {                               \
  size_t size;                         \
  size_t used;                         \
  TYPE *data;                          \
}

#if 0
// version that uses ,-operator so we get a status
// as a return value
#define da_init(da, init_size, init_used)         \
((da).data =                                      \
   checked_malloc(MAX(init_size, MIN_ARRAY_SIZE), \
                  *(da).data),                    \
 (da).size = (da).data ? init_size : 0,           \
 (da).used = (da).data ? init_used : 0,           \
 !!da.data)

#else

// version where we use a status variable
#define da_init(da, status, init_size, init_used)       \
do {                                                    \
 (da).data =                                            \
    checked_malloc(MAX(init_size, MIN_ARRAY_SIZE),      \
                   *(da).data);                         \
 (da).size = (da).data ? init_size : 0;                 \
 (da).used = (da).data ? init_used : 0;                 \
 status = !!da.data;                                    \
} while (0)

#endif

#define da_dealloc(da)                            \
do {                                              \
  free((da).data);                                \
  (da).data = 0;                                  \
  (da).size = (da).used = 0;                      \
} while (0)

#if 0
// typesafe version that uses a non-standard __typeof__ macro

#define da_resize(da, status, new_size)           \
do {                                              \
  size_t alloc_size =                             \
    MAX(new_size, MIN_ARRAY_SIZE);                \
  __typeof__(da.data) new_data =                  \
    checked_realloc((da).data,                    \
        alloc_size, *(da).data);                  \
  if (!new_data) { status = false; break; }       \
  (da).data = new_data;                           \
  (da).size = alloc_size;                         \
  (da).used = MIN((da).used, new_size);           \
  status = true;                                  \
} while (0)

#else
// using void *
#define da_resize(da, status, new_size)           \
do {                                              \
  size_t alloc_size =                             \
    MAX(new_size, MIN_ARRAY_SIZE);                \
  void *new_data = checked_realloc((da).data,     \
          alloc_size, *(da).data);                \
  if (!new_data) { status = false; break; }       \
  (da).data = new_data;                           \
  (da).size = alloc_size;                         \
  (da).used = MIN((da).used, new_size);           \
  status = true;                                  \
} while (0)

#endif

// simple append for simple arguments. It can handle
// da_append(int_da, 13, status);
// but not
// da_append(point_da, (point){.x = 0, .y = 2}, status);

#if 0

#define da_append(da, status, val)              \
do {                                            \
  if ((da).used == (da).size) {                 \
    if (at_max_len((da).size, *(da).data))      \
      { status = false; break; }                \
    size_t new_size =                           \
      capped_dbl((da).size, *(da).data);        \
    da_resize(da, status, new_size);            \
    if (!status) break;                         \
  }                                             \
  (da).data[(da).used++] = val;                 \
  status = true;                                \
} while (0)

#else

#define da_append(da, status, ...)              \
do {                                            \
  if ((da).used == (da).size) {                 \
    if (at_max_len((da).size, *(da).data))      \
      { status = false; break; }                \
    size_t new_size =                           \
      capped_dbl((da).size, *(da).data);        \
    da_resize(da, status, new_size);            \
    if (!status) break;                         \
  }                                             \
  (da).data[(da).used++] = __VA_ARGS__;         \
  status = true;                                \
} while (0)

#endif


struct point { double x, y; };

#if 0
// call it with a dynarray(point) * and you get an
// incompatible pointer type, because identical
// anonymous structs are not the same type
bool add_origin(dynarray(struct point) *da)
{
  bool status;
  da_append(
    *da, status,
    (struct point){ .x = 0, .y = 0 }
  );
  return status;
}

#else

typedef dynarray(struct point) point_array;
bool add_origin(point_array *da)
{
  bool status;
  da_append(
    *da, status,
    (struct point){ .x = 0, .y = 0 }
  );
  return status;
}

#endif


int main(void)
{
  bool success = true;
  dynarray(struct point) pda;
  da_init(pda, success, 0, 0);
  if (!success) goto error;

  for (int i = 0; i < 5; i++) {
    da_append(
      pda, success,
      (struct point){ .x = i, .y = -i }
    );
    if (!success) goto error;
  }

  for (int i = 0; i < da_len(pda); i++) {
    printf("<%.1f,%.1f> ",
      da_at(pda, i).x,
      da_at(pda, i).y
    );
  }
  printf("\n");

  da_dealloc(pda);
  return 0;

error:
  da_dealloc(pda);
  return 1;
}
