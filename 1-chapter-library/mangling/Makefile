CC=gcc
CPP=g++
SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)
EXE=random

all: $(EXE)

%.o: %.c %.h
	$(CC) -c $< -o $@ 	

$(EXE): main.cpp $(OBJ)
	$(CPP) $^ -o $@

clean:
	-rm $(OBJ) $(EXE)
