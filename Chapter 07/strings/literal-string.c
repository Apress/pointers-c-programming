#include <stdio.h>

int main(void)
{
  char *string = "foo";
  string[1] = 'O'; // Changing foo to fOo?
  printf("Succes! string is now '%s'\n", string);
  return 0;
}
