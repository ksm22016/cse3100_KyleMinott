# This is a comment line

#define variables so it is easier to make changes
CC      = gcc
CFLAGS  =-g -Wall -std=c99
TARGETS = ex-factorial catalan

.PHONY: all clean
.SUFFIXES:        # disable old implicit suffix rules (optional but helps)

all: $(TARGETS)

ex-factorial: ex-factorial.c
	$(CC) $(CFLAGS) -o $@ $<

catalan: catalan.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f ex-factorial catalan *.o a.out *~