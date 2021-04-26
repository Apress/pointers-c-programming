#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

char const *slice(char const *x, int i, int j)
{
  assert(i <= j);
  size_t len = j - i;

  char *new_buf = malloc(len + 1);
  if (!new_buf) return 0;

  strncpy(new_buf, x + i, len);
  new_buf[len] = '\0';
  return new_buf;
}

#define size_check_inc(var,x)             \
  { if (SIZE_MAX - (var) < (x))           \
      goto error;                         \
    (var) += (x);                         \
  }
#define size_check_inc_prod(var,x,y)      \
  { if ((x) > 0 &&                        \
        (SIZE_MAX - (var)) / (x) < (y))   \
      goto error;                         \
    (var) += (x) * (y);                   \
  }

char *replace_string(char const *x, int i, int j, char const *y)
{
  size_t xlen = strlen(x);
  size_t ylen = strlen(y);
  size_t len = xlen - (j - i) + 1; // 1 for zero terminal
  size_check_inc(len, ylen);

  char *new_buf = malloc(len);
  if (!new_buf) goto error;

  strncpy(new_buf, x, i);
  strcpy(new_buf + i, y);
  strcpy(new_buf + i + ylen, x + j);

  return new_buf;

error:
  return 0;
}

char *insert_string(char const *x, int i, char const *y)
{
  return replace_string(x, i, i, y);
}

char *delete_string(char const *x, int i, int j)
{
  return replace_string(x, i, j, "");
}

char *join_strings(char const *sep,
                   int n, char const *strings[n])
{
  size_t len = 1; // 1 for zero terminal
  size_t sep_len = strlen(sep);
  size_t reps = (n > 1) ? (n - 1) : 0;
  size_check_inc_prod(len, sep_len, reps);

  for (int i = 0; i < n; i++) {
    size_t string_len = strlen(strings[i]);
    size_check_inc(len, string_len);
  }

  char *new_buf = malloc(len);
  if (!new_buf) goto error;

#define append_string(src) \
    { for (char const *p = src; *p; p++) *dst++ = *p; }
  char *dst = new_buf;
  char const *xsep = "";
  for (int i = 0; i < n; i++) {
    append_string(xsep);
    append_string(strings[i]);
    xsep = sep;
  }
  *dst = '\0';
#undef append_string

  return new_buf;

error:
  return 0;
}



// using substrings

struct range {
  char *begin;
  char *end;
};

typedef struct range substr;
typedef struct range substr_iter;

// Creating substr
#define SUBSTR(b,e) \
  (substr){ .begin = (b), .end = (e) }

substr as_substr(char *s)
{
  char *x = (char *)s;
  while (*x) x++;
  return SUBSTR((char *)s, x);
}

// Use a null begin pointer as a special symbol
static substr const NULL_SUBSTR = { .begin = 0 };
#define null_substr(x)  ((x).begin == 0)
#define substr_len(x)   ((x).end - (x).begin)

substr find_occurrence(substr x, substr y)
{
  int n = substr_len(x);
  int m = substr_len(y);
  if (m > n) return NULL_SUBSTR;

  char *s = x.begin, *end = x.end - m;
  for (; s < end; s++) {
    if (strncmp(s, y.begin, m) == 0) {
      return SUBSTR(s, s + m);
    }
  }
  return NULL_SUBSTR;
}

// Iterator for non-overlapping occurrences of s
substr next_occurrence(substr_iter *iter,
                       substr s,
                       int overlaps)
{
  substr occ = find_occurrence(*iter, s);
  if (!null_substr(occ)) {
    // if there was an occurrence, we must update
    // the iterator
    iter->begin = overlaps ? occ.begin + 1 : occ.end;
  }
  return occ;
}

char *replace_all_occurrences(char const *z,
                              char const *x,
                              char const *y)
{
  substr ssz = as_substr((char *)z);
  substr ssx = as_substr((char *)x);
  substr ssy = as_substr((char *)y);
  size_t zlen = substr_len(ssz);
  size_t xlen = substr_len(ssx);
  size_t ylen = substr_len(ssy);

  // Compute the new string's length
  size_t len = zlen + 1; // + 1 for terminal
  substr_iter iter = ssz;
  for (substr occ = next_occurrence(&iter, ssx, 0);
       !null_substr(occ);
       occ = next_occurrence(&iter, ssx, 0)) {
    len -= xlen;
    size_check_inc(len, ylen);
  }
  char *new_buf = malloc(len);
  if (!new_buf) goto error;

#define copy_range(b, e) \
{ for (char const *p = (b); p != (e); p++) *dst++ = *p; }
  char const *src = z;
  char *dst = new_buf;
  iter = ssz;
  for (substr occ = next_occurrence(&iter, ssx, 0);
       !null_substr(occ);
       occ = next_occurrence(&iter, ssx, 0)) {
    copy_range(src, occ.begin);
    copy_range(ssy.begin, ssy.end);
    src = occ.end;
  }
  copy_range(src, z + zlen);
  *dst = '\0';
#undef copy_range

  return new_buf;

error:
  return 0;
}

int main(void)
{
  char const *x = "foobarbaz";
  char *y = (char *)slice(x, 3, 6);
  printf("y is %s\n", y);
  assert(strcmp(y, "bar") == 0);
  free(y);

  y = replace_string(x, 3, 6, "");
  printf("y is %s\n", y);
  assert(strcmp(y, "foobaz") == 0);
  free(y);

  y = replace_string(x, 3, 6, "quxqax");
  printf("y is %s\n", y);
  assert(strcmp(y, "fooquxqaxbaz") == 0);
  free(y);

  y = insert_string(x, 0, "XX");
  printf("y is %s\n", y);
  assert(strcmp(y, "XXfoobarbaz") == 0);
  free(y);

  y = delete_string(x, 0, 3);
  printf("y is %s\n", y);
  assert(strcmp(y, "barbaz") == 0);
  free(y);

  char const *strings[] = {
    "foo", "bar", "baz"
  };
  char *z = join_strings(":", sizeof strings / sizeof *strings, strings);
  printf("z = %s\n", z);
  free(z);

  char const *strings2[] = {
    "",
  };
  z = join_strings(":", sizeof strings2 / sizeof *strings2, strings2);
  printf("z = %s\n", z);
  free(z);

  x = "foobarbazbax";
  z = replace_all_occurrences(x, "ba", "");
  printf("z == %s\n", z);
  free(z);

  z = replace_all_occurrences(x, "a", "OOOO");
  printf("z == %s\n", z);
  free(z);

  z = replace_all_occurrences(x, "123", "OOOO");
  printf("z == %s\n", z);
  free(z);

  return 0;
}
