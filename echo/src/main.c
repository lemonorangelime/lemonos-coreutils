#include <sys/utsname.h>
#include <stdio.h>
#include <argv.h>
#include <runtime.h>

int main(int argc, char * argv[]) {
	for (int i = 1; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");
}
