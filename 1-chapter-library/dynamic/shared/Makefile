CC=gcc
LIB=libsolution.so
SRC=$(filter-out main.c, $(wildcard *.c))
EXE=solution

.PHONY: all clean

all: $(EXE)

$(LIB): $(SRC)
	$(CC) -shared -fPIC $^ -o $@

$(EXE): main.c $(LIB)
	$(CC) $< -L. -lsolution -Wl,-rpath,. -o $@	

clean:
	-rm $(LIB) $(EXE)
