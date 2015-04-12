#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "bufrand.h"

int randfd = -2;

// read from /dev/random into a large int buffer since rand() only returns ints
void bufrand(void *buf, size_t len) {
	if (randfd == -2 && ((randfd = open("/dev/random", O_RDONLY)) == -1)) {
		fprintf(stderr, "Couldn't open /dev/random\n");
		exit(1);
	}

	read(randfd, buf, len);
}