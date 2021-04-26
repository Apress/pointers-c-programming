#ifndef SUBSTR_H
#define SUBSTR_H

struct range {
  char *begin;
  char *end;
};

typedef struct range substr;
typedef struct range substr_iter;

// Creating substr
#define SUBSTR(b,e) \
  (substr){ .begin = (b), .end = (e) }

substr as_substr(char *s);
substr slice(char *s, int i, int j);

// Use a null begin pointer as a special symbol
static substr const NULL_SUBSTR = { .begin = 0 };
#define null_substr(x)  ((x).begin == 0)
#define empty_substr(x) ((x).begin == (x).end)
#define substr_len(x)   ((x).end - (x).begin)

// Creating a string from a substr.
// The caller is responsible for ensuring that
// the to buffer is large enough (i.e. at least
// substr_len(from) + 1 long).
char *substr_to_buf(char *to, substr from);

// IO
void print_substr(substr s);

// Mutating substrings to create strings
char zero_term(substr s);
void restore_term(substr s, char c);


// Basic operations
int substr_cmp(substr x, substr y);
void substr_rev(substr s);

// We write the charcters in from to to, but only
// part of from if to is shorter than from.
substr copy_substr(substr to, substr from);

// It is the caller's responsibility to check that
// x and y have the same length and do not overlap
void swap_substr(substr x, substr y);

// Remove substr y from substr x.
// It is the caller's responsibility to check that
// x is a subrange of out.
substr delete_substr(substr out, substr x, substr y);

// With this one, remember that you are modifying
// x, so other references to it, and subtrings in it
// will also be affected
substr delete_substr_inplace(substr x, substr y);

// Replace string x in z with the string in y.
// It is the caller's responsibility to ensure that
// x is a subrange of z and y is not contained
// in out.
substr replace_substr(substr out,
                      substr z, substr x,
                      substr y);

// It is the caller's responsibility to ensure that
// x is a subrange of z and that y is not contained
// in z.
substr replace_substr_inplace(substr z, substr x,
                              substr y);

// Insert substr y in substr x at index.
// It is the caller's responsibility to check that
// index is contined in x
substr insert_substr(substr out, substr x, size_t index, substr y);
substr insert_substr_inplace(substr x, size_t index, substr y);


substr next_word(substr_iter *iter);
substr copy_words(substr to, substr from);
substr compact_words(substr s);


// Find the first occurrence of the string y
// in the string x. Return a null-string if
// there isn't one.
substr find_occurrence(substr x, substr y);
// Iterator for non-overlapping occurrences of s
substr next_occurrence(substr_iter *iter, substr s, int overlaps);

#endif
