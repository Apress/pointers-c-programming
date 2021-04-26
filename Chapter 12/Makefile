
tests = simple reference reference-refactored iterative \
			  explicit-stack explicit-stack-embed morris parent
CFLAGS = -Wall -pedantic

all: $(tests)

$(tests): %: %.c
	cc $(CFLAGS) $< -o $@
