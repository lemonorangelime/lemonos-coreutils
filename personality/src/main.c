#include <runtime.h>
#include <stdio.h>
#include <string.h>
#include <types.h>
#include <argv.h>
#include <main.h>
#include <math.h>
#include <lctl.h>
#include <personality.h>
#include <multitasking.h>

void personality_callback(mypriv_t * priv, args_option_t * option, int present) {
	if (!present) {
		return;
	}
	switch (option->short_name) {
		case 'l': priv->type = PERSONALITY_LEMONOSv1;	return;
		case 'i': priv->type = PERSONALITY_LINUX;	return;
		case 'w': priv->type = PERSONALITY_NT;		return;
		case 'r': priv->type = PERSONALITY_ROADRUNNER;	return;
		case 't': priv->type = PERSONALITY_TEMPLEOS;	return;
	}
}

void callback(mypriv_t * priv, args_option_t * option, char * string) {
	priv->file = string;
}

int main(int argc, char * argv[]) {
	if (!__is_lemonos) {
		// not even gonna entertain you
		printf("This program can only be run on LemonOS-like kernels\n");
		return -1;
	}

	args_progspec_t spec[] = {
		"LemonOS coreutils", "1.0", "Lemon", NULL,
		"unlicense: <https://unlicense.org>",
		"This is public domain software: you are free to change and redistribute it.",
		"There is NO WARRANTY, to the extent permitted by law.",
		"Start process with selected personality."
	};
	args_option_t options[] = {
		{'l', "lemonos",	0, TYPE_BOOL,	0b10000000000000000,	personality_callback, .help="select LemonOS"},
		{'i', "linux",		0, TYPE_BOOL,	0b10000000000000000,	personality_callback, .help="select Linux"},
		{'w', "win",		0, TYPE_BOOL,	0b10000000000000000,	personality_callback, .help="select WinNT"},
		{'r', "roadrunneros",	0, TYPE_BOOL,	0b10000000000000000,	personality_callback, .help="select RoadrunnerOS"},
		{'t', "templeos",	0, TYPE_BOOL,	0b10000000000000000,	personality_callback, .help="select TempleOS"},
		{1,   NULL,		1, TYPE_STRING,	ARG_REQUIRED,		callback, .help="PROGRAM"},
	};
	int options_count = sizeof(options) / sizeof(options[0]);
	mypriv_t priv;
	memset(&priv, 0, sizeof(priv));
	priv.type = LINUX; // whatever
	priv.file = "NULL";

	args_setup(ARG_DEFAULT_TO_HELP);
	args_set_help_character('\0');
	args_load_spec(spec);
	if (args_parse(argc, argv, options_count, options, &priv)) {
		return -1;
	}

	int pid = lctl(LCTL_INITRD_LOAD, priv.file, 0, NULL);
	process_t * process = (process_t *) lctl(LCTL_GET_PCB, pid);
	process->personality = (personality_t *) lctl(LCTL_GET_PERSONALITY, priv.type);
	process->killed = 0;

	args_unsetup();
	return 0;
}
