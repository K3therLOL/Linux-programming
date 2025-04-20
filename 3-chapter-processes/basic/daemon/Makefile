CC=gcc
CFLAGS=-g -Wall
SRC=solution.c
EXE=solution

.PHONY: all clean

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(EXE)
