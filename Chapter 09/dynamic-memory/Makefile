
tests = malloc calloc aligned_alloc realloc \
        strings dynarray gapbuf

all: $(tests)

$(tests): %: %.c
	cc -std=c11 -Wall -pedantic $< -o $@
