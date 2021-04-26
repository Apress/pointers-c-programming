#ifndef REFCOUNT_H
#define REFCOUNT_H

#include <stdlib.h>

void *rc_alloc(size_t size, void cleanup(void *, void *));
void *incref(void *p);

// Use this one when decref'ing from a callback
void *decref_ctx(void *p, void *ctx);
// Use this one otherwise
#define decref(p) decref_ctx(p, 0)

// Annotation macros
#define borrows
#define takes
#define give(x) x


#endif
