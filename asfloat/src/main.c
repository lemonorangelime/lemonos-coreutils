#include <argv.h>
#include <main.h>
#include <stdio.h>
#include <string.h>
#include <types.h>

void callback(priv_t * priv, args_option_t * option, int arg, int has_arg) {
	priv->i = arg;
}

int main(int argc, char * argv[]) {
	args_progspec_t spec[] = {
		"LemonOS coreutils", "1.0", "Lemon", NULL,
		"unlicense: <https://unlicense.org>",
		"This is public domain software: you are free to change and redistribute it.",
		"There is NO WARRANTY, to the extent permitted by law.",
		"Convert integer to float."
	};
	args_option_t options[] = {
		{'\x01', "int", 1, TYPE_INT, ARG_ARGUMENT_REQUIRED, callback, .help="INTEGER"},
	};
	int options_count = sizeof(options) / sizeof(options[0]);
	args_setup(ARG_DEFAULT_TO_HELP);
	args_load_spec(spec);

	priv_t priv = {0};
	if (args_parse(argc, argv, options_count, options, &priv)) {
		return -1;
	}

	float * f = (float *) &priv.i;
	printf("%f\n", *f);

	args_unsetup();
	return 0;
}
