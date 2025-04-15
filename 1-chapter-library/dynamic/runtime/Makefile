CC=gcc
CFLAGS=-fPIC -ldl -Wl,-rpath,.
SRC=solution.c
EXE=solution

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $^ $(CFLAGS) -o $@ 

clean:
	rm $(EXE)
