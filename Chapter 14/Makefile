
lists = casting offset
tests = $(lists) tree-list
CFLAGS = -Wall -pedantic

all: $(tests)

$(lists): %: %.c list.o
	cc $(CFLAGS) $< list.o -o $@

tree-list: tree-list.c list.o stree.o
	cc $(CFLAGS) $< list.o stree.o -o $@

list.o: list.c list.h
stree.o: stree.c stree.h
