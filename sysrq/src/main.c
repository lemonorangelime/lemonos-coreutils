#include <multitasking.h>
#include <runtime.h>
#include <stdio.h>
#include <string.h>
#include <types.h>
#include <argv.h>
#include <ctype.h>
#include <events.h>
#include <lctl.h>

void callback(char * priv, args_option_t * option, char * arg) {
	if (strlen(arg) != 1) {
		printf("Unrecognised key %s\n", arg);
		return;
	}
	*priv = *arg;
}

uint8_t key2code(char key) {
	if (key >= '0' && key <= '9') {
		return 0x02 + (key - '0');
	}
	char keymap[26] = {0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1f, 0x14, 0x16, 0x2f, 0x11, 0x2d, 0x15, 0x2c};
	if (key >= 'a' && key <= 'z') {
		return keymap[key - 'a'];
	}
	return 0;
}

int ustrcmp(uint16_t * x, uint16_t * y) {
	int i = 0;
	while (x[i] != 0 && y[i] != 0) {
		if (x[i] != y[i]) {
			return 1;
		}
		i++;
	}
	return x[i] != 0 || y[i] != 0;
}

typedef void (* send_t)(event_t * event);
void send_keystroke(process_t * process, uint8_t keycode, keyboard_held_t * held, int pressed) {
	kbd_event_t key = {0, EVENT_KEYBOARD, keycode, pressed, held};
	switch (keycode) {
		case 0x36: held->rshift = pressed; break;
		case 0x56: held->sysrq = pressed; break;
		case 0x57: held->super = pressed; break;
	}
	send_t send = process->recv_global_event;
	send((event_t *) &key);
}

process_t * find_sysrq() {
	int pid = 0;
	while (pid < 1024) {
		process_t * process = (process_t *) lctl(LCTL_GET_PCB, pid);
		if (ustrcmp(process->name, u"sysrqd") == 0) {
			return process;
		}
		pid++;
	}
	return NULL;
}

int main(int argc, char * argv[]) {
	args_progspec_t spec[] = {
		"LemonOS coreutils", "1.0", "Lemon", NULL,
		"unlicense: <https://unlicense.org>",
		"This is public domain software: you are free to change and redistribute it.",
		"There is NO WARRANTY, to the extent permitted by law.",
		"Inject system request keystrokes."
	};
	args_option_t options[] = {
		{1, NULL, 1, TYPE_STRING, ARG_REQUIRED, callback, .help="KEY"},
	};
	int options_count = sizeof(options) / sizeof(options[0]);
	char chr = 0;

	args_setup(ARG_DEFAULT_TO_HELP);
	args_load_spec(spec);
	if (args_parse(argc, argv, options_count, options, &chr)) {
		return -1;
	}

	if (chr == 0) {
		return -1;
	}

	if (!__is_lemonos) {
		FILE * fp = fopen("/proc/sysrq-trigger", "w");
		fwrite(&chr, 1, 1, fp);
		fclose(fp);
		return 0;
	}

	char lower = tolower(chr);
	uint8_t code = key2code(lower);
	keyboard_held_t held;
	memset(&held, 0, sizeof(keyboard_held_t));

	process_t * process = find_sysrq();
	send_keystroke(process, 0x57, &held, 1);
	send_keystroke(process, 0x56, &held, 1);
	send_keystroke(process, 0x56, &held, 0);

	if (lower != chr) {
		send_keystroke(process, 0x36, &held, 1);
	}
	send_keystroke(process, code, &held, 1);
	send_keystroke(process, code, &held, 0);
	if (lower != chr) {
		send_keystroke(process, 0x36, &held, 0);
	}

	send_keystroke(process, 0x57, &held, 0);


	args_unsetup();
	return 0;
}
