
tests = intrc lists extension-lists

all: $(tests) strees

$(tests): %: %.c
	cc -Wall -pedantic -O3 $< -o $@

strees: refcount.c strees.c

%.o: %.c
	cc -Wall -pedantic -O3 $< -o $@
