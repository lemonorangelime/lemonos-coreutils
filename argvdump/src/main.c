#include <stdio.h>

int main(int argc, char * argv[]) {
	printf("argc == %d\n", argc);
	printf("argv == %r\n", argv);
	for (int i = 0; i < argc; i++) {
		printf("argv[%d] == %r(%s)\n", i, argv[i], argv[i]);
	}
	return 0;
}
