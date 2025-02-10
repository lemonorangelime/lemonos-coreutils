#include <stdio.h>
#include <sched.h>
#include <string.h>
#include <runtime.h>
#include <graphics.h>

#define TASKBAR_HEIGHT 28

int main(int argc, char * argv[]) {
	// since this whole thing is inherently LemonOS specific we can freely use the LemonOS APIs
	if (!__is_lemonos) {
		printf("This program only runs on LemonOS!\n");
		return -1;
	}
	framebuffer_spec_t spec;
	if (!get_framebuffer_spec(&spec)){
		printf("VIDEO DRIVER ERROR!\n");
		return -1;
	}

	/* goals: elimnate number of rects (prefer pointers)
	 * also get rid of the `fake_root_window` bullshit
	 */
	window_t * active_window = NULL;
	memset32(spec.fb, 0xffff00ff, spec.width * spec.height);
	
}
