
#include <stdio.h>

#define smallint(i)     (int *)((i) << 1 | 1)
#define get_smallint(p) ((int)(p) >> 1)
#define is_smallint(p)  ((int)(p) & 1)
#define get_int(p) \
  (is_smallint(p) ? get_smallint(p) : (*(p)))

int main(void)
{
  int *p1 = smallint(1);
  int *p2 = smallint(2);
  printf("%d + %d = %d\n",
          get_int(p1), get_int(p2),
          get_int(p1) + get_int(p2));

  int i3 = 3;
  p2 = &i3;
  printf("%d + %d = %d\n",
         get_int(p1), get_int(p2),
         get_int(p1) + get_int(p2));

  return 0;
}
