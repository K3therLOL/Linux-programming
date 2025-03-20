CC=gcc
CFLAGS=-L. -lHello 

SRC=$(filter-out main.c, $(wildcard *.c))
HEAD=$(wildcard *.h)
OBJ=$(SRC:%.c=%.o)

LIB=libHello.a
EXE=hello

.PHONY: all clean

all: $(EXE)

%.o: %.c $(HEAD)
	$(CC) -c $< -o $@

$(LIB): $(OBJ)	
	ar rcs $@ $^
	rm $^

$(EXE): $(LIB) $(OBJ)
	$(CC) main.c -L. -lHello -o $@

clean:
	-rm $(LIB) $(EXE)
