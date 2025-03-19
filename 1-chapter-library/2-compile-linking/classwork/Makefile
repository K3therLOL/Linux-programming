CC=gcc
SRC=$(wildcard *.c)
HEAD=$(wildcard *.h)
OBJ=$(SRC:%.c=lib/%.o)
EXE=hello

.PHONY: all clean
all: $(EXE)

lib/%.o: %.c $(HEAD)
	$(CC) -c $< -o $@

$(EXE): $(OBJ)
	$(CC) $^ -o $@

clean:
	rm $(OBJ) $(EXE)
