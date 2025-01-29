#include <stdio.h>

int main(int argc, char * argv[]) {
	if (argc < 2) {
		printf("Usage: cat [filename]\n");
		return -1;
	}
	char chunk[512];
	int length = 511;
	int fd = open(argv[1], 0);
	while (length == 511) {
		length = read(fd, chunk, 511);
		chunk[length] = 0;
		printf("%s", chunk);
	}
	close(fd);
	return 0;
}
