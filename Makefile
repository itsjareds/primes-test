BINS=naive miller-rabin2

CC=gcc
CFLAGS=-Wall -O3

.PHONY: clean all print $(BINS)

all: $(BINS)

$(BINS): %: bin/%

bin/%: %.c driver.c | bin
	$(CC) $(CFLAGS) -o $@ driver.c bufrand.c $< -lgmp

bin:
	mkdir bin

print-%: ; @echo $* = $($*)

clean:
	rm -rf bin/