#include <graphics.h>
#include <sched.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char * argv[]) {
	window_t * window = create_window(u"Gradient", u"gradient", 400, 256);
	rect_2d_t * rect = &window->rect;
	uint8_t bandr = 0xff;
	uint8_t bandg = 0xff;
	uint8_t bandb = 0xff;
	uint8_t bandw = 0xff;
	uint32_t * fb = rect->fb;
	for (int i = 0; i < 256; i++) {
		uint32_t band1_colour = 0xff000000 | (bandr << 16);
		uint32_t band2_colour = 0xff000000 | (bandg << 8);
		uint32_t band3_colour = 0xff000000 | bandb;
		uint32_t band4_colour = 0xff000000 | (bandw << 16) | (bandw << 8) | bandw;
		memset32(fb, band1_colour, 100);
		memset32(fb + 100, band2_colour, 100);
		memset32(fb + 200, band3_colour, 100);
		memset32(fb + 300, band4_colour, 100);
		bandr--;
		bandg--;
		bandb--;
		bandw--;
		fb += 400;
	}
	while (1) {
		sched_yield();
	}
	return 0;
}
