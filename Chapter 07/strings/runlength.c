#include <stdio.h>
#include <string.h>
#include <assert.h>

char const *skip(char const *x)
{
  char c = *x; assert(c != '\0');
  while (*x == c) x++;
  return x;
}

void runlength_encode(char const * restrict input,
                      char * restrict output)
{
  while (*input) {
    char c = *input;
    char const *next = skip(input);
    int length = next - input;
    output += sprintf(output, "%d%c", length, c);
    input = next;
  }
}

int main(void)
{
  char const *x = "aaaabbbbbbbaabbbcbbccccc";
  char buffer[2 * strlen(x) + 1];

  runlength_encode(x, buffer);
  printf("%s\n", buffer);

  return 0;
}
