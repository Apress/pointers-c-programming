
tests = creation comparison reverse copying \
        delete-replace word-iterator \
				searching sort occurrences

all: $(tests) run_all

$(tests): %: %.o substr.o substr.h
	cc $< substr.o -o $@

run_all: $(tests)
	for f in $(tests); do	echo $$f; ./$$f; done
