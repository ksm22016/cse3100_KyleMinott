# This is a comment line

#define variables so it is easier to make changes
CC     :=gcc
CFLAGS :=-g -Wall -std=c99
TARGETS:=ex-factorial catalan

.PHONY: all clean

all: $(TARGETS)

ex-factorial: ex-factorial.o
	$(CC) $(CFLAGS) -o $@ $^

ex-factorial.o: ex-factorial.c
	$(CC) $(CFLAGS) -c $<

# catalan
catalan: catalan.o
	$(CC) $(CFLAGS) -o $@ $^

catalan.o: catalan.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o *~ $(TARGETS) a.out