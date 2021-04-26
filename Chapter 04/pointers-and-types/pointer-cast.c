#include <assert.h>

void f1(void)
{
  int *ip = (int *)0x12345;
  void *p = ip;
  int *ip2 = p;
  assert(ip == ip2);
}

void f2(void)
{
  int *ip = (int *)0x12345;
  volatile int *ip2 = ip;
  int *ip3 = (int *)ip2;
  assert(ip == ip2);
  assert(ip == ip3);
}

void f3(void)
{
  int i;
  char *x = (char *)&i;
  assert((char *)&i == x);
}

int main(void)
{
  f1();
  f2();
  f3();
  return 0;
}
