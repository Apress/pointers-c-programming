#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct dynarray {
  size_t size;
  size_t used;
  int *data;
};

#define da_at(da,i)     (da)->data[(i)]
#define da_len(da)      (da)->used

bool da_init   (struct dynarray *da,
                size_t init_size,
                size_t init_used);
void da_dealloc(struct dynarray *da);
bool da_append (struct dynarray *da,
                int val);

#endif
