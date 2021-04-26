
tests = void-array buf-array code-generation inline \
	heap flex-array heap-ref

all: $(tests)

$(tests): %: %.c
	cc -std=c11 -Wall -pedantic $< -o $@
