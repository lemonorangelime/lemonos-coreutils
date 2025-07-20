#include <stdio.h>
#include <fcntl.h>

int main(int argc, char * argv[]) {
	for (int i = 1; i < argc; i++) {
		creat(argv[i], 0644);
	}
	return 0;
}
