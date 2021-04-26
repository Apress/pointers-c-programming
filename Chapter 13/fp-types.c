
#include <stdio.h>

void f(void) {}
void (*fp)(void) = f;

double g(int x, float y) { return x + y; }
double (*gp)(int, float) = &g;

void (* complex(int x, int (*p)(int)) )(void)
{
  printf("%d\n", p(x));
  return f;
}

typedef void (*void_fn)(void);
typedef int (*int_fn)(int);

void_fn simple(int x, int_fn p)
{
  printf("%d\n", p(x));
  return f;
}

// h has type int_fn
int h(int x) { return 2 * x; }

int main(void)
{

  // cast
  fp = ( void (*)(void) ) g;

  // address or not...
  fp = f;
  fp = &f;

  complex(42, h);

  return 0;
}
