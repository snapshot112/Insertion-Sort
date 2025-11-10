# Set the default compiler to gcc
CC = gcc
# CC = clang

# Use C11, debugging and warnings flags
define CFLAGS
-std=c11 \
-g3 \
-fsanitize=address \
-Wpedantic \
-Wall \
-Wextra \
-Wformat=2 \
-Wsign-conversion \
-Wswitch-enum \
-Wno-switch-default \
-Wunused \
-Wfloat-equal \
-Wconversion \
-Wpointer-arith \
-Wtype-limits \
-Wcast-qual \
-Wenum-compare \
-Wsizeof-pointer-memaccess \
`pkg-config --cflags check` \
-Wstrict-prototypes
endef

# Turn on the address sanitizer and link math library
LDFLAGS = -fsanitize=address -lm

# Flags needed for the check library
CHECK_LDFLAGS = $(LDFLAGS) `pkg-config --libs check`

PROG = mysort
TESTS = check_list check_extra malloc-test

all: $(PROG) $(TESTS)

valgrind: LDFLAGS=-lm
valgrind: CFLAGS=-Wall
valgrind: $(PROG)

list.o: list.c list.h

mysort: main.o list.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(PROG) $(TESTS)

tarball: insertion_sort_submit.tar.gz

insertion_sort_submit.tar.gz: main.c list.c list.h Makefile
	tar -czf $@ $^

check_list: check_list.o list.o
	$(CC) -o $@ $^ $(CHECK_LDFLAGS)

check_extra: check_extra.o list.o
	$(CC) -o $@ $^ $(CHECK_LDFLAGS)

malloc-test: LDFLAGS=$(shell pkg-config --libs check) -ldl -fsanitize=address
malloc-test: CFLAGS=-std=c11 -g3 -Wall `pkg-config --cflags check` -fsanitize=address
malloc-test: malloc-test.o list.o
	$(CC) -o $@ $^ $(LDFLAGS)

check: all
	@echo "Checking linked list implementation..."
	./check_list
	@echo
	@echo "Extra checking of linked list implementation..."
	./check_extra
	@echo
	@echo "\nChecking insertion sort implementation..."
	./check_sort.sh
	@echo
	@echo "\nTesting if errors on functions are checked"
	./malloc-test
