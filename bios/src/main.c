#include <runtime.h>
#include <stdio.h>
#include <string.h>
#include <types.h>
#include <argv.h>
#include <main.h>
#include <math.h>
#include <ctype.h>

void callback(mypriv_t * priv, args_option_t * option, int value, int present) {
	if (!present) {
		return;
	}
	int shift = isupper(option->short_name) * 8;
	switch (option->short_name) {
		case '\1':
			priv->irq = value;
			return;
		case '\0':
			break; // handle X register
		case 'a': // L/H registers are named
		case 'A':
			priv->ax |= value << shift;
			return;
		case 'b':
		case 'B':
			priv->bx |= value << shift;
			return;
		case 'c':
		case 'C':
			priv->cx |= value << shift;
			return;
		case 'd':
		case 'D':
			priv->dx |= value << shift;
			return;
	}

	if (strcmp(option->long_name, "ax") == 0) {
		priv->ax = value;
	} else if (strcmp(option->long_name, "bx") == 0) {
		priv->bx = value;
	} else if (strcmp(option->long_name, "cx") == 0) {
		priv->cx = value;
	} else if (strcmp(option->long_name, "dx") == 0) {
		priv->dx = value;
	}
}

int main(int argc, char * argv[]) {
/*	if (!__is_lemonos){
		printf("Yeah no way\n");
		return -1;
	} */
	args_progspec_t spec[] = {
		"LemonOS coreutils", "1.0", "Lemon", NULL,
		"unlicense: <https://unlicense.org>",
		"This is public domain software: you are free to change and redistribute it.",
		"There is NO WARRANTY, to the extent permitted by law.",
		"Call bios functions from cmdline."
	};
	args_option_t options[] = {
		{0,   "ax", 1, TYPE_INT, 0,		callback,	.help="set AX register"},
		{0,   "bx", 1, TYPE_INT, 0,		callback,	.help="set BX register"},
		{0,   "cx", 1, TYPE_INT, 0,		callback,	.help="set CX register"},
		{0,   "dx", 1, TYPE_INT, 0,		callback,	.help="set DX register"},

		{'a', "al", 1, TYPE_INT, 0,		callback,	.help="set AL register [AX low]"},
		{'b', "bl", 1, TYPE_INT, 0,		callback,	.help="set BL register [BX low]"},
		{'c', "cl", 1, TYPE_INT, 0,		callback,	.help="set CL register [CX low]"},
		{'d', "dl", 1, TYPE_INT, 0,		callback,	.help="set DL register [DL low]"},

		{'A', "ah", 1, TYPE_INT, 0,		callback,	.help="set AH register [AX high]"},
		{'B', "bh", 1, TYPE_INT, 0,		callback,	.help="set BH register [BX high]"},
		{'C', "ch", 1, TYPE_INT, 0,		callback,	.help="set CH register [CX high]"},
		{'D', "dh", 1, TYPE_INT, 0,		callback,	.help="set DH register [Dx high]"},
		{1,   NULL, 1, TYPE_INT, ARG_REQUIRED,	callback,	.help="IRQ"},
	};
	int options_count = sizeof(options) / sizeof(options[0]);
	mypriv_t state;
	memset(&state, 0, sizeof(mypriv_t));

	args_setup(ARG_DEFAULT_TO_HELP);
	args_set_help_character('\0');
	args_load_spec(spec);
	if (args_parse(argc, argv, options_count, options, &state)) {
		return -1;
	}

	uint8_t code[16] = {
		0xb8, 0xff, 0xff,
		0xbb, 0xff, 0xff,
		0xb9, 0xff, 0xff,
		0xba, 0xff, 0xff,
		0xcd, 0x80,
		0x0f, 0x0b
	};
	memcpy(&code[1], &state.ax, 2);
	memcpy(&code[4], &state.bx, 2);
	memcpy(&code[7], &state.cx, 2);
	memcpy(&code[10], &state.dx, 2);
	code[13] = state.irq;

	for (int i = 0; i < 16; i++) {
		printf("0x%x, ", code[i]);
	}
	printf("\n");

	args_unsetup();
	return 0;
}
