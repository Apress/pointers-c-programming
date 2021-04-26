
tests = fp-types \
  higher-order-functions \
	callback \
	string-iter \
	stack \
	expr \
	gen int-tag gen-tagged
CFLAGS = -Wall -pedantic

all: $(tests)

higher-order-functions: %: %.c list.o
	cc $(CFLAGS) $< list.o -o $@

stack: %: %.c list.o dynarray.o
	cc $(CFLAGS) $< list.o dynarray.o -o $@

list.o: list.c list.h
dynarray.o: dynarray.c dynarray.h
