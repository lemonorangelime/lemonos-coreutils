#include <stdio.h>
#include <fcntl.h>
#include <graphics.h>

uint32_t fbspec_get_size(framebuffer_spec_t * spec) {
	switch (spec->bpp) {
		case 4:
			return (spec->width * spec->height) >> 1;
		case 8:
			return spec->width * spec->height;
		case 15:
		case 16:
			return (spec->width * spec->height) * 2;
		case 24:
			return (spec->width * spec->height) * 3;
		case 32:
			return (spec->width * spec->height) * 4;
	}
}

int main(int argc, char * argv[]) {
	framebuffer_spec_t spec;
	get_framebuffer_spec(&spec);

	char chunk[512];
	int fd = open(argv[1], 0);
	if (fd <= 0) {
		return -1;
	}
	write(fd, spec.fb, fbspec_get_size(&spec));
	close(fd);
	return 0;
}
