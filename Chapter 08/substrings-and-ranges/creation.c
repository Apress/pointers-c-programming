#include <stdio.h>
#include <string.h>
#include "substr.h"

int main(void)
{
  do {
    char buf[] = "foobarbaz";
    substr string = as_substr(buf);
    substr bar = slice(buf, 3, 6);

    print_substr(string); printf("\n");
    print_substr(bar); printf("\n");

    printf("my favorite string is \"%s\"\n",
           string.begin);

    char tmp[substr_len(bar) + 1];
    substr_to_buf(tmp, bar);
    printf("my second favorite string is \"%s\"\n",
           tmp);

    char c = zero_term(bar);
    printf("my second favorite string is \"%s\"\n",
           bar.begin);
    restore_term(bar, c);
  } while(0);

  do {
    char s[] = "foobarbaz";
    substr ss = as_substr(s);
    printf("%zu %zu\n", strlen(s), substr_len(ss));

    printf("slicing \""); print_substr(ss); printf("\"");
    ss = slice(s, 3, 6);
    printf(" gives us \""); print_substr(ss); printf("\"\n");
    printf("%zu %zu\n", strlen(s), substr_len(ss));

    char buf[substr_len(ss) + 1];
    substr_to_buf(buf, ss);
    printf("%s\n", buf);

    char c = zero_term(ss);
    printf("%s\n", ss.begin);
    restore_term(ss, c);
  } while(0);

  return 0;
}
