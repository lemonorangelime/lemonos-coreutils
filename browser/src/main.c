#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <types.h>
#include <argv.h>

void callback(void * priv, args_option_t * option, char * arg) {
}

int main(int argc, char * argv[]) {
	args_progspec_t spec[] = {
		"LemonOS coreutils", "1.0", "Lemon", NULL,
		"unlicense: <https://unlicense.org>",
		"This is public domain software: you are free to change and redistribute it.",
		"There is NO WARRANTY, to the extent permitted by law.",
		"Browse the web."
	};
	args_option_t options[] = {
		{'\1', "URL", 0, TYPE_STRING, 0, callback, .help="URL"},
	};
	int options_count = sizeof(options) / sizeof(options[0]);
	args_setup(ARG_NONE_REQUIRED);
	args_load_spec(spec);
	if (args_parse(argc, argv, options_count, options, NULL)) {
		return -1;
	}

	
}
