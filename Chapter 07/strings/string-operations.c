#include <stdio.h>
#include <string.h>

int strlen_index(char const *s)
{
  int i = 0;
  while (s[i])
    i++;
  return i;
}

int strlen_pointer(char const *s)
{
  char const *x = s;
  while (*x)
    x++;
  return x - s;
}

char *string_copy(char *output, char const *input)
{
  char const *x = input;
  char *y = output;
  for ( ; *x ; x++, y++ )
    *y = *x;
  *y = '\0';
  return y;
}

char *string_copy_(char *output, char const *input)
{
  while (*input) {
    *output++ = *input++;
  }
  *output = '\0';
  return output;
}

char *string_copy__(char *output, char const *input)
{
  while ( (*output = *input) ) {
    output++; input++;
  }
  return output;
}

char *string_copy___(char *output, char const *input)
{
  while ( (*output++ = *input++) ) ;
  return output;
}

char *string_copy____(char *output, const char *input)
{
  do {
    *output++ = *input;
  } while (*input++);
  return output;
}

void reverse_string(char *s)
{
  char *left = s;
  char *right = s + strlen(s) - 1;
  for ( ; left < right; left++, right--) {
    char c = *left;
    *left = *right;
    *right = c;
  }
}

int main(void)
{
  char const *string = "hello, world\n";
  int n = strlen(string);
  printf("The string has length %d, and we need %d chars to represent it\n", n, n + 1);
  char buffer[n + 1];
  string_copy___(buffer, string);
  printf("%s", buffer);
  string_copy____(buffer, string);
  printf("%s", buffer);

  reverse_string(buffer);
  printf("%s", buffer);

  return 0;
}
