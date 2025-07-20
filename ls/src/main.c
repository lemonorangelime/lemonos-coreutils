#include <stdio.h>
#include <fcntl.h>
#include <types.h>
#include <dirent.h>
#include <argv.h>

void callback(void * priv, args_option_t * option, char * arg) {
}

int main(int argc, char * argv[]) {
	DIR * dir = opendir(".");
	struct dirent * dirent;
	while (dirent = readdir(dir)) {
		if (dirent->d_name[0] == '.') {
			continue;
		}
		printf("%s  ", dirent->d_name);
	}
	printf("\n");
	closedir(dir);
	return 0;
}
