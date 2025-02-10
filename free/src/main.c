#include <sys/sysinfo.h>
#include <stdio.h>
#include <string.h>
#include <types.h>
#include <argv.h>
#include <main.h>
#include <math.h>

void format_callback(mypriv_t * priv, args_option_t * option, int present) {
	if (!present) {
		return;
	}
	char * name = option->long_name;
	int length = strlen(name) - 1;
	if (length == 4) {
		priv->format = BYTES;
		return;
	}
	char end = name[length];
	char start = name[0];
	if (end == 'i') {
		priv->si = 0;
	}
	switch (start) {
		case 'k':
			priv->format = KILO;
			return;
		case 'm':
			priv->format = MEGA;
			return;
		case 'g':
			priv->format = GIGA;
			return;
		case 't':
			priv->format = TERA;
			return;
		case 'p':
			priv->format = PETA;
			return;
	}
}

void callback(mypriv_t * priv, args_option_t * option, int present) {
	if (!present) {
		return;
	}
	switch (option->short_name) {
		case 'h':
			priv->human = 1;
			return;
		case '\0':
			priv->si = 0;
			return;
		case 'l':
			priv->lohi = 1;
			return;
		case 't':
			priv->total = 1;
			return;
		case 'w':
			priv->wide = 1;
			return;
	}
}

void print_header(mypriv_t * priv) {
	if (priv->wide) {
		puts("               total        used        free      shared     buffers       cache   available\n");
		return;
	}
	puts("               total        used        free      shared  buff/cache   available\n");
}

void pn(uint32_t prescaled, mypriv_t * priv) {
	uint64_t unit = (uint64_t) priv->info->mem_unit;
	uint64_t number = (uint64_t) prescaled * unit;
	int divisor = priv->si ? 1000 : 1024;
	if (priv->human) {
		uint64_t scaled = memory_scale(number, divisor);
		char suffix = memory_suffix(number, divisor);

		char buffer[64];
		int tab = 11 - (priv->si ? (suffix == 'B' ? 1 : 2) : 1);
		ulldtoa(scaled, buffer, 10);
		tab -= strlen(buffer);
		while (tab-- > 0) { putchar(' '); }
		printf(" %l%c", &scaled, suffix);
		if (suffix != 'B') {
			putchar(priv->si ? 'i' : '\0');
		}
		return;
	}
	switch (priv->format) {
		case KILO: number /= (divisor); break;
		case MEGA: number /= (divisor * 1000); break;
		case GIGA: number /= (divisor * 1000000); break;
		case TERA: number /= (divisor * 1000000000); break;
		case PETA: number /= (divisor * 1000000000000); break;
	}
	char buffer[64];
	int tab = 11;
	ulldtoa(number, buffer, 10);
	tab -= strlen(buffer);
	while (tab-- > 0) { putchar(' '); }
	printf(" %l", &number);
}

void print_caches(mypriv_t * priv, sysinfo_t * info) {
	pn(info->bufferram, priv);
	pn(0, priv);
	if (priv->wide) {
		pn(0, priv);
		return;
	}
}

int main(int argc, char * argv[]) {
	args_progspec_t spec[] = {
		"LemonOS coreutils", "1.0", "Lemon", NULL,
		"unlicense: <https://unlicense.org>",
		"This is public domain software: you are free to change and redistribute it.",
		"There is NO WARRANTY, to the extent permitted by law.",
		"Display amount of free and used memory in the system."
	};
	args_option_t options[] = {
		{'b', "bytes",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in bytes"},
		{0,   "kilo",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in kilobytes"},
		{0,   "mega",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in megabytes"},
		{0,   "giga",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in gigabytes"},
		{0,   "tera",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in terabytes"},
		{0,   "peta",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in petabytes"},
		{'k', "kibi",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in kibibytes"},
		{'m', "mebi",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in mebibytes"},
		{0,   "gibi",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in gibibytes"},
		{0,   "tebi",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in tebibytes"},
		{0,   "pebi",	0, TYPE_BOOL, 0b10000000000000000,	format_callback, .help="show output in pebibytes"},
		{'h', "human",	0, TYPE_BOOL, 0,			callback, .help="show human-readable output"},
		{0,   "si",	0, TYPE_BOOL, 0,			callback, .help="use powers of 1000 not 1024"},
		{'l', "lohi",	0, TYPE_BOOL, 0,			callback, .help="show detailed low and high memory statistics"},
		{'t', "total",	0, TYPE_BOOL, 0,			callback, .help="show total for RAM + swap"},
		{'w', "wide",	0, TYPE_BOOL, 0,			callback, .help="wide output"},
	};
	int options_count = sizeof(options) / sizeof(options[0]);
	mypriv_t priv;
	memset(&priv, 0, sizeof(priv));
	priv.si = 1;
	priv.format = KILO;

	args_setup(ARG_DEFAULT_TO_HELP | ARG_NONE_REQUIRED);
	args_set_help_character('\0');
	args_load_spec(spec);
	if (args_parse(argc, argv, options_count, options, &priv)) {
		return -1;
	}

	sysinfo_t info;
	sysinfo(&info);
	priv.info = &info;

	print_header(&priv);
	printf("Mem:    "); pn(info.totalram, &priv); pn(info.totalram - info.freeram, &priv); pn(info.freeram, &priv); pn(info.sharedram, &priv);
	print_caches(&priv, &info);
	putchar('\n');

	printf("Swap:   "); pn(info.totalswap, &priv); pn(info.totalswap - info.freeswap, &priv); pn(info.freeswap, &priv);
	putchar('\n');

	if (priv.total) {
		printf("Total:  "); pn(info.totalram + info.totalswap, &priv); pn((info.totalram + info.totalswap) - (info.freeram + info.freeswap), &priv); pn(info.freeram + info.freeswap, &priv);
		putchar('\n');
	}


	/*
	printf("buffer: %u %u\n", info.bufferram, info.mem_unit);
	printf("swap: %u %u\n", info.totalswap, info.mem_unit);
	printf("free swap: %u %u\n", info.freeswap, info.mem_unit);
	*/

	args_unsetup();
	return 0;
}
