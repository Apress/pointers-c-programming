#include <stdio.h>
#include <stdlib.h>

struct rc_int {
  int refcount;
  int value;
};

struct rc_int *new_rc_int(int i)
{
  struct rc_int *p = malloc(sizeof *p);
  if (p) {
    *p = (struct rc_int){
       .refcount = 1, .value = i
     };
  }
  return p;
}

void free_rc_int(struct rc_int *i)
{
  printf("Freeing %d\n", i->value);
  free(i);
}

struct rc_int *incref(struct rc_int *p)
{
  if (p) p->refcount++;
  return p;
}

struct rc_int *decref(struct rc_int *p)
{
  if (p && --p->refcount == 0) {
    free_rc_int(p);
    return 0;
  }
  return p;
}

int main(void)
{
  // Ignoring malloc errors...
  struct rc_int *i = new_rc_int(42);
  struct rc_int *j = incref(i);
  decref(i); // decrements...
  decref(j); // decrements and deletes...

  return 0;
}
