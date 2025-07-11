CC=gcc
CFLAGS=-g -Wall -O0
SRC=solution.c
EXE=solution

.PHONY: all clean

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(EXE)
