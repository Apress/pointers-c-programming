int main(void)
{
  int i = 42;
  int *ip = &i;
  void *vp = ip;
  char *cp = vp;

  return 0;
}
