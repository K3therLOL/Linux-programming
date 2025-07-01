CC=gcc
CFLAGS=-Wall -Wextra -O0 -g
SRC=solution.c
EXE=solution
PIPES=in1 in2

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(EXE) $(PIPES)
