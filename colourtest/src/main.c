#include <stdio.h>

int main() {
	for (int i = 30; i < 38; i++) {
		printf("\x1b[0;%dm██\x1b[0m", i);
	}
	printf("\n");
}
