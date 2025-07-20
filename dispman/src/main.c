#include <runtime.h>
#include <stdio.h>
#include <string.h>
#include <types.h>
#include <argv.h>
#include <main.h>
#include <math.h>
#include <displays.h>

void callback(mypriv_t * priv, args_option_t * option, int value) {
	switch (option->short_name) {
		case 'w':
			priv->width = value;
			return;
		case 'h':
			priv->height = value;
			return;
		case 'b':
		case 'c':
			priv->bpp = value;
			return;
	}
}

int main(int argc, char * argv[]) {
	if (!__is_lemonos) {
		printf("This program only runs on LemonOS-like kernels.\n");
		return -1;
	}
	args_progspec_t spec[] = {
		"LemonOS coreutils", "1.0", "Lemon", NULL,
		"unlicense: <https://unlicense.org>",
		"This is public domain software: you are free to change and redistribute it.",
		"There is NO WARRANTY, to the extent permitted by law.",
		"Display configuration changer."
	};
	args_option_t options[] = {
		{'w', "width",	1, TYPE_INT, 0, callback, .help="change display width"},
		{'h', "height",	1, TYPE_INT, 0, callback, .help="change display height"},
		{'b', "bpp",	1, TYPE_INT, 0, callback, .help="change display BPP"},
		{'c', "crunch",	1, TYPE_INT, 0, callback, .help="change display BPP (alias)"},
	};
	int options_count = sizeof(options) / sizeof(options[0]);
	mypriv_t priv;
	memset(&priv, 0, sizeof(priv));
	priv.width = 0;
	priv.height = 0;
	priv.bpp = 0;

	args_setup(ARG_DEFAULT_TO_HELP | ARG_NONE_REQUIRED);
	args_set_help_character('H');
	args_load_spec(spec);
	if (args_parse(argc, argv, options_count, options, &priv)) {
		return -1;
	}

	display_t * display = display_get_default();
	if (priv.bpp != 0) {
		display_crunch(display, priv.bpp);
	}

	if (priv.width != 0 || priv.height != 0) {
		int width = (priv.width == 0) ? display->width : priv.width;
		int height = (priv.height == 0) ? display->height : priv.height;
		display_resize(display, width, height);
	}

	args_unsetup();
	return 0;
}
