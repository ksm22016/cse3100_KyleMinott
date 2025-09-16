# This is a comment line

#define variables so it is easier to make changes
CC      := gcc
CFLAGS  := -g -Wall -Wextra -std=c99
TARGETS := ex-factorial catalan

.PHONY: all clean
.SUFFIXES:        # disable old implicit suffix rules (optional but helps)

all: $(TARGETS)

# ---- Link from .o files ONLY ----
ex-factorial: ex-factorial.o
	$(CC) $(CFLAGS) -o $@ $^

catalan: catalan.o
	$(CC) $(CFLAGS) -o $@ $^

# ---- Compile .c -> .o ----
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ $(TARGETS) a.out