CC=gcc
CFLAGS=-Wall -Wextra -g -O0
SRC=solution.c
EXE=solution

.PHONY: all clean

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(EXE)
