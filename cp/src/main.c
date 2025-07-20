#include <stdio.h>
#include <fcntl.h>
#include <types.h>
#include <argv.h>

void copy_file(char * input, char * output) {
	char chunk[512];
	int length = 512;
	int infd = open(input, O_RDONLY | O_CREAT, 0644);
	int outfd = open(output, O_WRONLY | O_CREAT, 0644);
	while (length == 512) {
		length = read(infd, chunk, 512);
		write(outfd, chunk, length);
	}
	close(infd);
	close(outfd);
}

int main(int argc, char * argv[]) {
	copy_file(argv[1], argv[2]);
	return 0;
}
