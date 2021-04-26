#include <stdio.h>

/*  1 */ int factorial(int n)
/*  2 */ {
/*  3 */  if (n <= 1) {
/*  4 */    return 1;
/*  5 */  } else {
/*  6 */    int rec_result = factorial(n - 1);
/*  7 */    return n * rec_result;
/*  8 */  }
/*  9 */ }

/* 10 */ void print_factorial(int n)
/* 11 */ {
/* 12 */  for (int i = 0; i < n; i++) {
/* 13 */    int fact = factorial(i);
/* 14 */    printf("%d! = %d\n", i, fact);
/* 15 */  }
/* 16 */ }

/* 17 */ int main(void)
/* 18 */ {
/* 19 */   print_factorial(4);
/* 20 */   return 0;
/* 21 */ }
