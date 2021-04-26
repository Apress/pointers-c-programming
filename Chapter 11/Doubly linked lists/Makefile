
tests = doubly-linked-lists sorting
CFLAGS = -Wall -pedantic

all: $(tests)

$(tests): %: %.c list.o
	cc $(CFLAGS) $< list.o -o $@

list.o: list.c list.h
