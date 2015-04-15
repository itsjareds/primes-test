BINS=naive miller-rabin2 miller-rabin3
MPIBINS=brute brute-gmp

CC=gcc
CFLAGS=-O3

MPICC=mpicc
MPIFLAGS=
GMPFLAGS=-DUSE_GMP

.SUFFIXES:

.PHONY: clean all $(BINS)

all: $(BINS) $(MPIBINS)

$(BINS): %: bin/%

$(MPIBINS): %: bin/mpi/%

bin/mpi/%-gmp: %-gmp.c | bin/mpi
	$(MPICC) $(CFLAGS) $(MPIFLAGS) $(GMPFLAGS) -o $@ $< mpigmp/mpi_gmp.c -lgmp

bin/mpi/%: %.c | bin/mpi
	$(MPICC) $(CFLAGS) $(MPIFLAGS) -o $@ $<

bin/%: %.c driver.c | bin
	$(CC) $(CFLAGS) -o $@ driver.c bufrand.c $< -lgmp

bin:
	mkdir bin

bin/mpi: | bin
	mkdir bin/mpi

clean:
	rm -rf bin/
