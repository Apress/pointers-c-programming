#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "substr.h"

substr as_substr(char *s)
{
  char *x = s;
  while (*x) x++;
  return SUBSTR(s, x);
}

substr slice(char *s, int i, int j)
{
  assert(i <= j);
  return SUBSTR(s + i, s + j);
}


// Creating a string from a substr.
// The caller is responsible for ensuring that
// the to buffer is large enough (i.e. at least
// substr_len(from) + 1 long).
char *substr_to_buf(char *to, substr from)
{
  while (from.begin != from.end) {
    *to++ = *from.begin++;
  }
  *to = '\0';
  return to;
}

// IO
void print_substr(substr s)
{
  while (s.begin != s.end) {
    putchar(*s.begin++);
  }
}

// Mutating substrings to create strings
char zero_term(substr s)
{
  char c = *s.end;
  *s.end = '\0';
  return c;
}

void restore_term(substr s, char c)
{
  *s.end = c;
}



// Basic operations
int substr_cmp(substr x, substr y)
{
  while (x.begin != x.end && y.begin != y.end) {
    if (*x.begin < *y.begin) return -1;
    if (*x.begin > *y.begin) return +1;
    x.begin++; y.begin++;
  }
  // We've reached the end of one of the substrings.
  // If they had the same length, they are equal,
  // otherwise, the shorter string is the smallest
  if (x.begin < x.end) return +1; // x is longer
  if (y.begin < y.end) return -1; // y is longer
  return 0; // the strings are equal
}

void substr_rev(substr s)
{
  if (empty_substr(s)) return;
  char * restrict x = s.begin;
  char * restrict y = s.end - 1;
  for (; x < y; x++, y--) {
    char c = *x; *x = *y; *y = c; // swap
  }
}

// It is the caller's responsibility to check that
// x and y have the same length and do not overlap
void swap_substr(substr x, substr y)
{
  char * restrict p = x.begin;
  char * restrict q = y.begin;
  for (; p != x.end && q != y.end; p++, q++) {
    char c = *p; *p = *q; *q = c; // swap
  }
}


#define MIN(a,b) (((a)<(b)) ? (a) : (b))

// We write the charcters in from to to, but only
// part of from if to is shorter than from.
substr copy_substr(substr to, substr from)
{
  size_t n = MIN(substr_len(to), substr_len(from));
  // copy right cannot handle empty strings, so bail out here
  if (n == 0) return to;

  // Technically, the comparison is only well defined if the pointers
  // are into the same allocated buffer, but we are always allowed to
  // test, and we do not care about the result if they are not in the
  // same buffer, so it doesn't matter.
  if (to.begin < from.begin) { // copy left
      char * restrict y = to.begin;
      char * restrict x = from.begin, *xend = from.begin + n;
      while (x < xend) {
        *y++ = *x++;
      }
  } else { // copy right
    char * restrict y = to.begin + n;
    char * restrict x = from.begin + n, *xbeg = from.begin;
    do {
      *(--y) = *(--x);
    } while (x > xbeg);
  }

  return SUBSTR(to.begin + n, to.end);
}





// Replace string x in z with the string in y.
// It is the caller's responsibility to ensure that
// x is a subrange of z and y is not contained
// in out.
substr replace_substr(substr out,
                      substr z, substr x,
                      substr y)
{
  substr z_before   = SUBSTR(z.begin, x.begin);
  substr z_after    = SUBSTR(x.end, z.end);

  size_t ylen       = substr_len(y);
  size_t outlen     = substr_len(out);
  size_t x_beg_idx  = x.begin - z.begin;
  size_t y_end_idx  = MIN(x_beg_idx + ylen, outlen);
  substr out_before = SUBSTR(out.begin, out.begin + x_beg_idx);
  substr out_after  = SUBSTR(out.begin + y_end_idx, out.end);
  substr out_y      = SUBSTR(out.begin + x_beg_idx,
                             out.begin + y_end_idx);

  copy_substr(out_before, z_before);
  char *ret_end = copy_substr(out_after, z_after).begin;
  copy_substr(out_y, y);

  return SUBSTR(out.begin, ret_end);
}

// It is the caller's responsibility to ensure that
// x is a subrange of z and that y is not contained
// in z.
substr replace_substr_inplace(substr z, substr x,
                              substr y)
{
  size_t zlen = substr_len(z);
  size_t ylen = substr_len(y);
  size_t x_beg_idx = x.begin - z.begin;
  size_t y_end_idx = MIN(x_beg_idx + ylen, zlen);
  char *y_end = z.begin + y_end_idx;

  substr in_after  = SUBSTR(x.end, z.end);
  substr out_after = SUBSTR(y_end, z.end);
  substr out_y     = SUBSTR(x.begin, y_end);

  char *ret_end = copy_substr(out_after, in_after).begin;
  copy_substr(out_y, y);

  return SUBSTR(z.begin, ret_end);
}

// Insert substr y in substr x at index.
// It is the caller's responsibility to check that
// index is contained in x
substr insert_substr(substr out, substr x, size_t index, substr y)
{
  assert(index <= substr_len(x));
  char *p = x.begin + index;
  return replace_substr(out, x, SUBSTR(p, p), y);
}

// It is the caller's responsibility to check that
// index is contained in x
substr insert_substr_inplace(substr x, size_t index, substr y)
{
  assert(index < substr_len(x));
  char *p = x.begin + index;
  return replace_substr_inplace(x, SUBSTR(p, p), y);
}

// Delete substr y from substr x
// It is the caller's responsibility to check that
// y is contained in x
substr delete_substr(substr out, substr x, substr y)
{
  return replace_substr(out, x, y, as_substr(""));
}

substr delete_substr_inplace(substr x, substr y)
{
  return replace_substr_inplace(x, y, as_substr(""));
}

// Word iteration
char *skip_word(char *x)
{
  while (*x && isalpha(*x))
    x++;
  return x;
}

char *find_word(char *x)
{
  while (*x && !isalpha(*x))
    x++;
  return x;
}

substr next_word(substr_iter *iter)
{
  char *begin = find_word(iter->begin);
  if (*begin == '\0') {
    // no more words
    return NULL_SUBSTR;
  }
  char *end = skip_word(begin);
  iter->begin = end;
  return SUBSTR(begin, end);
}

// Compacting over iterator...
substr copy_words(substr to, substr from)
{
  // remember where we started
  char *begin = to.begin;

  // sep is used to put spaces between
  // words but not before the first word
  substr sep = as_substr(""); // empty string
  substr_iter iter = from;

  for (substr word = next_word(&iter);
       !null_substr(word);
       word = next_word(&iter)) {
    to = copy_substr(to, sep);
    to = copy_substr(to, word);

    // after the first iteration, sep should always
    // be space
    sep = as_substr(" ");
  }

  return SUBSTR(begin, to.begin);
}

substr compact_words(substr s)
{
  return copy_words(s, s);
}


// Find the first occurrence of the string y
// in the string x. Return a NULL string if
// there isn't one.
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
