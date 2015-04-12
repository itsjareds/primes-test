BINS=naive
CC=gcc
CFLAGS=-Wall -g

.PHONY: clean all

all: $(BINS)

$(BINS): %: %.c driver.c
	$(CC) $(CFLAGS) -o $@ driver.c $<

clean:
	rm -rf $(BINS)