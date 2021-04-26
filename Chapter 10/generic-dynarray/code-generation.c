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

#define da_at(da,i)     (da)->data[(i)]
#define da_len(da)      (da)->used

#define GEN_DYNARRAY_TYPE(TYPE)         \
  typedef struct {                      \
    size_t size;                        \
    size_t used;                        \
    TYPE *data;                         \
  } TYPE##_dynarray;

#define GEN_DYNARRAY_PROTOTYPES(TYPE)         \
bool TYPE##_da_init   (TYPE##_dynarray *da,   \
                       size_t init_size,      \
                       size_t init_used);     \
void TYPE##_da_dealloc(TYPE##_dynarray *da);  \
bool TYPE##_da_resize (TYPE##_dynarray *da,   \
                       size_t new_size);      \
bool TYPE##_da_append (TYPE##_dynarray *da,   \
                       TYPE val);


#define GEN_DYNARRAY_FUNCTIONS(TYPE)          \
bool TYPE##_da_init(TYPE##_dynarray *da,      \
                    size_t init_size,         \
                    size_t init_used)         \
{                                             \
  assert(init_size >= init_used);             \
  init_size = MAX(init_size, MIN_ARRAY_SIZE); \
  da->data =                                  \
    checked_malloc(init_size, *da->data);     \
  da->size = (da->data) ? init_size : 0;      \
  da->used = (da->data) ? init_used : 0;      \
  return !!da->data;                          \
}                                             \
                                              \
void TYPE##_da_dealloc(TYPE##_dynarray *da)   \
{                                             \
  free(da->data);                             \
  da->data = 0;                               \
  da->size = da->used = 0;                    \
}                                             \
                                              \
bool TYPE##_da_resize(TYPE##_dynarray *da,    \
                      size_t new_size)        \
{                                             \
  size_t alloc_size =                         \
    MAX(new_size, MIN_ARRAY_SIZE);            \
  TYPE *new_data =                            \
    checked_realloc(da->data,                 \
        alloc_size, *da->data);               \
  if (!new_data) return false;                \
  da->data = new_data;                        \
  da->size = alloc_size;                      \
  da->used = MIN(da->used, new_size);         \
  return true;                                \
}                                             \
                                              \
bool TYPE##_da_append(TYPE##_dynarray *da,    \
                      TYPE val)               \
{                                             \
  if (da->used == da->size) {                 \
    if (at_max_len(da->size, *da->data))      \
      return false;                           \
    size_t new_size =                         \
      capped_dbl(da->size, *da->data);        \
    int resize_success =                      \
      TYPE##_da_resize(da, new_size);         \
    if (!resize_success) return false;        \
  }                                           \
  da->data[da->used++] = val;                 \
  return true;                                \
}

// goes in .h file
GEN_DYNARRAY_TYPE(int)
GEN_DYNARRAY_PROTOTYPES(int)

// goes in .c file
GEN_DYNARRAY_FUNCTIONS(int)


GEN_DYNARRAY_TYPE(double)
GEN_DYNARRAY_PROTOTYPES(double)
GEN_DYNARRAY_FUNCTIONS(double)

#if 0
// This will fail because of space
GEN_DYNARRAY_TYPE(long long)

struct foo { int i, j; };
GEN_DYNARRAY_TYPE(struct foo)
#endif

// this is fine because of typedef
typedef struct { double x, y; } point;
GEN_DYNARRAY_TYPE(point)
GEN_DYNARRAY_PROTOTYPES(point)
GEN_DYNARRAY_FUNCTIONS(point)


int main(void)
{
  int_dynarray da;

  int success = int_da_init(&da, 4, 4);
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
    if (!int_da_append(&da, 10 * (i + 1))) {
      printf("allocation error");
      break;
    }
  }
  printf("current length %zu\n", da_len(&da));
  for (int i = 0; i < da_len(&da); i++) {
    printf("%d ", da_at(&da, i));
  }
  printf("\n");

  int_da_dealloc(&da);




  double_dynarray dda;

  success = double_da_init(&dda, 4, 4);
  if (!success) {
    printf("allocation error\n");
  }

  for (int i = 0; i < da_len(&dda); i++) {
    da_at(&dda, i) = (double)i;
  }
  for (int i = 0; i < da_len(&dda); i++) {
    printf("%.1f ", da_at(&dda, i));
  }
  printf("\n");

  printf("current length %zu\n", da_len(&dda));
  for (int i = 0; i < 10; i++) {
    if (!double_da_append(&dda, 10.0 * (i + 1))) {
      printf("allocation error");
      break;
    }
  }
  printf("current length %zu\n", da_len(&dda));
  for (int i = 0; i < da_len(&dda); i++) {
    printf("%.1f ", da_at(&dda, i));
  }
  printf("\n");

  double_da_dealloc(&dda);

  point_dynarray pda;
  point_da_init(&pda, 0, 0);
  for (int i = 0; i < 5; i++) {
    point_da_append(&pda, (point){ .x = i, .y = i });
  }
  for (int i = 0; i < da_len(&pda); i++) {
    point *p = &da_at(&pda, i);
    printf("<%.1f, %.1f>\n", p->x, p->y);
  }
  point_da_dealloc(&pda);

  return 0;
}
