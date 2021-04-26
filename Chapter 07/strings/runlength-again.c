#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

char const *skip(char const *x)
{
  char c = *x; assert(c != '\0');
  while (*x == c) x++;
  return x;
}

bool runlength_encode_n(char const * restrict input,
                        char * restrict output,
                        int n)
{
  while (*input) {
    printf("n == %d\n", n);
    char c = *input;
    char const *next = skip(input);
    int length = next - input;
    int used = snprintf(output, n, "%d%c", length, c);
    output += used; n -= used;
    if (n < 0) return false; // we couldn't write it all
    input = next;
  }
  return true; // Success
}

int main(void)
{
  char const *x = "aaaabbbbbbbaabbbcbbccccc";
  int n = 10;
  char buffer[n + 1];

  bool did_we_make_it = runlength_encode_n(x, buffer, n);
  if (did_we_make_it) {
    printf("We encoded the entire string.\n");
  } else {
    printf("We only got a prefix.\n");
  }
  printf("Encoding: %s\n", buffer);

  return 0;
}
