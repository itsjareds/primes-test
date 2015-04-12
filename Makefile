BINS=naive

CC=gcc
CFLAGS=-Wall

.PHONY: clean all print $(BINS)

all: $(BINS)

$(BINS): %: bin/%

bin/%: %.c driver.c | bin
	$(CC) $(CFLAGS) -o $@ driver.c $<

bin:
	mkdir bin

print-%: ; @echo $* = $($*)

clean:
	rm -rf bin/