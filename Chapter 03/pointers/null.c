#include <math.h>
#include <float.h>
#include <stdio.h>

typedef struct vector {
  double x;
  double y;
  double z;
} vector;

double vector_length(vector *v)
{
  double x = v->x, y = v->y, z = v->z;
  return sqrt(x*x + y*y * z*z);
}

void print_vector(vector const *v)
{
  if (!v) {
    printf("NULL\n");
  } else {
    double x = v->x, y = v->y, z = v->z;
    printf("<%.2f, %.2f, %.2f>\n", x, y, z);
  }
}

vector *shortest(int n, vector *vectors[n])
{
  if (n < 1) return 0; // Return a NULL pointer

  vector *shortest = vectors[0];
  double shortest_length = vector_length(shortest);
  for (int i = 1; i < n; ++i) {
    vector *v = vectors[i];
    double length = vector_length(v);
    if (length < shortest_length) {
      shortest = v;
      shortest_length = length;
    }
  }

  return shortest;
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

  vector const longest = {
     .x = DBL_MAX, .y = DBL_MAX, .z = DBL_MAX
   };
   vector const *v = shortest(0, vectors);
   v = v ? v : &longest;
   print_vector(v);


  return 0;
}
