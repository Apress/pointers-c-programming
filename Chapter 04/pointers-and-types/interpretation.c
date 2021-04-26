#include <stdio.h>

int main(void)
{
 printf("sizes: double = %zu, long = %zu, int = %zu, char = %zu\n",
        sizeof(double), sizeof(long), sizeof(int), sizeof(char));

 double d;
 double *dp = &d;
 long   *lp = (long *)&d;
 int    *ip = (int *)&d;
 char   *cp = (char *)&d;
 printf("dp == %p, lp = %p\nip == %p, cp == %p\n\n", dp, lp, ip, cp);

 d = 42.0;
 printf("*dp == %.20f, *lp == %ld, *ip == %d, *cp == %d\n",
        *dp, *lp, *ip, *cp);

 *ip = 4200;
 printf("*dp == %.20f, *lp == %ld, *ip == %d, *cp == %d\n",
        *dp, *lp, *ip, *cp);

 *cp = 42;
 printf("*dp == %.20f, *lp == %ld, *ip == %d, *cp == %d\n",
        *dp, *lp, *ip, *cp);

 return 0;
}
