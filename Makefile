BINS=naive
CC=gcc
CFLAGS=-Wall

.PHONY: clean all

all: $(BINS)

$(BINS): %: %.c driver.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ driver.c $<

clean:
	rm -rf bin/