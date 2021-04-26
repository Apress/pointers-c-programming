#include <math.h>
#include <float.h>
#include <stdio.h>

typedef struct vector {
  double x;
  double y;
  double z;
} vector;

void print_vector(vector const *v)
{
  double x = v->x, y = v->y, z = v->z;
  printf("<%.2f, %.2f, %.2f>\n", x, y, z);
}

double vector_length(vector *v)
{
  double x = v->x, y = v->y, z = v->z;
  return sqrt(x*x + y*y * z*z);
}
vector *shortest(int n, vector *vectors[n])
{
  vector *shortest = &(vector){
    .x = DBL_MAX, .y = DBL_MAX, .z = DBL_MAX
  };
  double shortest_length = vector_length(shortest);

  printf("%p %p\n", (void *)shortest, (void *)&shortest_length);

  for (int i = 0; i < n; ++i) {
    vector *v = vectors[i];
    double length = vector_length(v);
    if (length < shortest_length) {
      shortest = v;
      shortest_length = length;
    }
  }

  return shortest;
}

void trash_stack(void)
{
  volatile char x[1000];
  for (int i = 0; i < 1000; i++) {
    x[i] = 0;
  }
}

int main(void)
{
  vector *vectors[] = {
    &(vector){ .x = 10.0, .y = 13.0, .z = 42.0 },
    &(vector){ .x = -1.0, .y = 32.0, .z = 15.0 },
    &(vector){ .x =  0.0, .y =  3.0, .z =  1.0 }
  };

  print_vector(shortest(3, vectors));
  print_vector(shortest(2, vectors));
  print_vector(shortest(1, vectors));
  print_vector(shortest(0, vectors)); // BOOOM!!!

  vector *v = shortest(0, vectors);
  print_vector(v);
  trash_stack();
  print_vector(v);

  return 0;
}
