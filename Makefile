# This is a comment line

#define variables so it is easier to make changes
CC     :=gcc
CFLAGS :=-g -Wall -std=c99
TARGETS:=ex-factorial catalan

.PHONY: all clean

all: $(TARGETS)
# ex-factorial
ex-factorial: ex-factorial.o
	$(CC) $(CFLAGS) -o $@ $^
	
# catalan
catalan: catalan.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o *~ $(TARGETS) a.out