#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void main() {
	int a_write = open("A", O_WRONLY);

	lseek(a_write, 0, SEEK_SET);
	write(a_write, "1", 1);
	lseek(a_write, 10000, SEEK_SET);
	write(a_write, "1", 1);
	lseek(a_write, 0, SEEK_END);
	write(a_write, "1", 1);

	close(a_write);
}
