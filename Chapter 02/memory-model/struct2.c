#include <stdio.h>
#include <stdalign.h>

struct data {
  int i;
  char c[9];
};

int main(void)
{
  printf("sizeof components == %zu\n",
         sizeof(char[9]) + sizeof(int));
  printf("sizeof(struct data) == %zu\n",
         sizeof(struct data));
  printf("\n");
  printf("alignof(struct data) == %zu\n",
         alignof(struct data));
  printf("alignment of int == %zu\n",
         alignof(int));
  printf("alignment of char[9] == %zu\n",
         alignof(char[9]));

  return 0;
}
