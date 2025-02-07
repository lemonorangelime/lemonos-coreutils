#include <stdio.h>
#include <types.h>
#include <argv.h>

void read_file(char * name) {
	char chunk[512];
	int length = 511;
	int fd = open(name, 0);
	while (length == 511) {
		length = read(fd, chunk, 511);
		chunk[length] = 0;
		printf("%s", chunk);
	}
	close(fd);
}

void callback(void * priv, args_option_t * option, char * arg) {
}

int main(int argc, char * argv[]) {
	args_progspec_t spec[] = {
		"LemonOS coreutils", "1.0", "Lemon", NULL,
		"unlicense: <https://unlicense.org>",
		"This is public domain software: you are free to change and redistribute it.",
		"There is NO WARRANTY, to the extent permitted by law.",
		"Argument test program."
	};
	args_option_t options[] = {
		{'A', "show-all",		0, TYPE_BOOL,	0, callback, .help="equivalent to -vET"},
		{'b', "number-nonblank",	0, TYPE_BOOL,	0, callback, .help="number nonempty output lines"},
		{'e', NULL,			0, TYPE_BOOL,	0, callback, .help="equivalent to -vE"},
		{'E', "show-ends",		0, TYPE_BOOL,	0, callback, .help="display $ at end of each line"},
		{'n', "number",			0, TYPE_BOOL,	0, callback, .help="number all output lines"},
		{'s', "squeeze-blank",		0, TYPE_BOOL,	0, callback, .help="suppress repeated empty output lines"},
		{'t', NULL,			0, TYPE_BOOL,	0, callback, .help="equivalent to -vT"},
		{'T', "show-tabs",		0, TYPE_BOOL,	0, callback, .help="display TAB characters as ^I"},
		{'v', "show-nonprinting",	0, TYPE_BOOL,	0, callback, .help="escape non-printable characters"},
		{1,   "input",			1, TYPE_STRING,	0, callback, .help="FILE"}, // our positional
	};
	int options_count = sizeof(options) / sizeof(options[0]);

	args_setup(ARG_DEFAULT_TO_HELP | ARG_STACK_POSITIONALS);
	args_load_spec(spec);
	if (args_parse(argc, argv, options_count, options, NULL)) {
		return -1;
	}
	args_unsetup();

	read_file(argv[1]);
	return 0;
}
