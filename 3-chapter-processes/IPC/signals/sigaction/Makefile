CC=gcc
CFLAGS=-O0 -g -Wall -Wextra
SRC=solution.c
EXE=solution

.PHONY: all clean

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(EXE)
