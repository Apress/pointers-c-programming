#include <stdio.h>
#include <assert.h>
#include "substr.h"

int main(void)
{

  printf("deleting...\n");
  do {
    char x_buf[] = "foobar", z_buf[] = "foobar";
    substr x = as_substr(x_buf);
    substr z = as_substr(z_buf);
    substr y = slice(x_buf, 1, 3);

    printf("deleting oo from %s\n", x.begin);
    print_substr(x); printf(" -> ");
    substr res = delete_substr(z, x, y);
    print_substr(res); printf(" # "); print_substr(z); printf("\n");

    printf("then inserting XX at position zero.\n");
    res = insert_substr(z, x, 0, as_substr("XX"));
    print_substr(res); printf(" # "); print_substr(z); printf("\n");

    print_substr(x); printf(" -> ");
    res = delete_substr_inplace(x, y);
    print_substr(res); printf(" # "); print_substr(x); printf("\n");
    res = insert_substr_inplace(x, 0, as_substr("YY"));
    print_substr(res); printf(" # "); print_substr(x); printf("\n");
  } while(0);
  printf("\n");

  printf("replacing...\n");
  do {
    char out_buf[] = "foobarbazqux";
    substr out = as_substr(out_buf);
    char z_buf[] = "foobarbaz";
    substr z = as_substr(z_buf);
    substr x = find_occurrence(z, as_substr("bar"));
    substr y = as_substr("qux");

    print_substr(z); printf(" -> ");
    substr res = replace_substr(out, z, x, y);
    // fooquxbaz # fooquxbazqux
    print_substr(res); printf(" # "); print_substr(out); printf("\n");

  } while(0);
  do {
    char out_buf[] = "foobarbazqux";
    substr out = as_substr(out_buf);
    char z_buf[] = "foobarbaz";
    substr z = as_substr(z_buf);
    substr x = find_occurrence(z, as_substr("bar"));
    substr y = as_substr("X");

    print_substr(z); printf(" -> ");
    substr res = replace_substr(out, z, x, y);
    // fooXbaz # fooXbazazqux
    print_substr(res); printf(" # "); print_substr(out); printf("\n");
  } while(0);
  do {
    char out_buf[] = "foobarbazqux";
    substr out = as_substr(out_buf);
    char z_buf[] = "foobarbaz";
    substr z = as_substr(z_buf);
    substr x = find_occurrence(z, as_substr("bar"));
    substr y = as_substr("XXXX");

    print_substr(z); printf(" -> ");
    substr res = replace_substr(out, z, x, y);
    // fooXXXXbaz # fooXXXXazqux
    print_substr(res); printf(" # "); print_substr(out); printf("\n");
  } while(0);


  do {
    char z_buf[] = "foobarbazqux";
    substr z = as_substr(z_buf);
    substr x = find_occurrence(z, as_substr("bar"));
    substr y = as_substr("qax");
    substr res;

    printf("starting string: %s\n", z_buf);

    printf("inplace bar/qax -> ");
    res = replace_substr_inplace(z, x, y);
    // fooqaxbazqux # fooqaxbazqux
    print_substr(res); printf(" # "); print_substr(z); printf("\n");
    assert(substr_cmp(res, as_substr("fooqaxbazqux")) == 0);

    printf("inplace fo/XXXX -> ");
    x = slice(z_buf, 0, 2);
    y = as_substr("XXXX");
    res = replace_substr_inplace(z, x, y);

    print_substr(res); printf(" # "); print_substr(z); printf("\n");
    assert(substr_cmp(res, as_substr("XXXXoqaxbazq")) == 0);

    printf("inplace XXXX/YYY -> ");
    x = slice(z_buf, 0, 4);
    y = as_substr("YYY");
    res = replace_substr_inplace(z, x, y);

    print_substr(res); printf(" # "); print_substr(z); printf("\n");
    assert(substr_cmp(res, as_substr("YYYoqaxbazq")) == 0);
    assert(substr_cmp(z, as_substr("YYYoqaxbazqq")) == 0);
  } while(0);

  printf("\n");

  printf("appending...\n");
  do {
    char x_buf[] = "foobar", z_buf[] = "foobarxxx";
    substr x = as_substr(x_buf);
    substr z = as_substr(z_buf);
    substr res = insert_substr(z, x, substr_len(x), as_substr("QUX"));
    print_substr(res); printf(" # "); print_substr(z); printf("\n");
  } while(0);

  return 0;
}
