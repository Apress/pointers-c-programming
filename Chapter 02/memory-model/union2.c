#include <stdio.h>

union data {
  char c[9];
  double d;
};

#define MAX(a,b) (((a)>(b))?(a):(b))

int main(void)
{
  union data data;
  printf("sizeof(data) == %zu\n", sizeof data);
  printf("max size of components == %zu\n",
         MAX(sizeof data.c, sizeof data.d));

  printf("data at   %p\n",(void *)&data);
  printf("data.c at %p\n", (void *)&data.c);
  printf("data.d at %p\n", (void *)&data.d);

  return 0;
}
