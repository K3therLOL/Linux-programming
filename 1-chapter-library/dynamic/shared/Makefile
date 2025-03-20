CC=gcc
LIB=libsolution.so
SRC=solution.c

.PHONY: all clean

all: $(LIB)

$(LIB): $(SRC)
	$(CC) -shared -fPIC $^ -o $@
clean:
	-rm $(LIB)
