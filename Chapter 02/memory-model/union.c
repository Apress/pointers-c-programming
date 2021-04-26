#include <stdio.h>

union data {
  char c;
  int i;
  double d;
};

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MAX3(a,b,c) MAX((a),MAX((b),(c)))

int main(void)
{
  union data data;
  printf("sizeof(data) == %zu\n", sizeof data);
  printf("max size of components == %zu\n",
         MAX3(sizeof data.c, sizeof data.i, sizeof data.d));

  printf("data at   %p\n",(void *)&data);
  printf("data.c at %p\n", (void *)&data.c);
  printf("data.i at %p\n", (void *)&data.i);
  printf("data.d at %p\n", (void *)&data.d);

  return 0;
}
