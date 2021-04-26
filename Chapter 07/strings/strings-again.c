#include <stdio.h>
#include <stdbool.h>

bool string_copy_n(char *output, const char *input, int n)
{
  while ( n-- && (*output = *input) ) {
    output++; input++;
  }
  return !*input; // Did we reach the end?
}

int main(void)
{
  const char *string = "hello, world\n";

  int n = 10;
  char buffer[n];
  bool all = string_copy_n(buffer, string, sizeof buffer);
  if (all) {
    printf("We copied it all\n");
    printf("%s", buffer);
  } else {
    printf("We didn't copy it all\n");
    buffer[n - 1] = '\0';
    printf("%s", buffer);
  }

  return 0;
}
