#include <graphics.h>
#include <runtime.h>
#include <stdio.h>
#include <argv.h>
#include <string.h>
#include <stdlib.h>
#include <types.h>
#include <sched.h>
#include <events.h>
#include <locale.h>
#include <lctl.h>
#include <ansi.h>
#include <stdout.h>
#include <multitasking.h>
#include <sys/prctl.h>
#include <main.h>
#include <lexer.h>

void render_frame(window_t * window, terminal_spec_t * terminal, uint16_t * text) {
	rect_2d_t * rect = &window->rect;
	memset32(rect->fb, terminal->background_colour, rect->size.width * rect->size.height);
	for (int y = 0; y < terminal->cheight; y++) {
		for (int x = 0; x < terminal->cwidth; x++) {
			uint16_t codepoint = text[(y * terminal->cwidth) + x];
			if (codepoint == 0) {
				continue;
			}
			font_draw(font_get(terminal->font, codepoint), 0xffffffff, x * 8, y * 16, rect->size.width, rect->fb);
		}
	}
}

void set_icon(window_t * window) {
	uint32_t terminal_icon[]  = {
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 
		0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000
	};
	memcpy32(window->taskbar->icon, terminal_icon, 256);
}

void callback(terminal_spec_t * spec, args_option_t * option, int arg, int has_arg) {
	if (!has_arg) {
		return;
	}
	char name = option->short_name;
	if ((name == 'w' || name == 'h') && arg < 16) {
		arg = 16;
	}
	switch (name) {
		case 'w':
			spec->width = arg;
			return;
		case 'h':
			spec->height = arg;
			return;
		case 'n':
			spec->has_colour = !arg;
			return;
	}
}

void handle_argv(terminal_spec_t * termspec, int argc, char * argv[]) {
	args_progspec_t spec[] = {
		"LemonOS coreutils", "1.0", "Lemon", NULL,
		"unlicense: <https://unlicense.org>",
		"This is public domain software: you are free to change and redistribute it.",
		"There is NO WARRANTY, to the extent permitted by law.",
		"Terminal emulator."
	};
	args_option_t options[] = { // our pile of garbage to pass to args_parse()
		{'w', "width",		1, TYPE_INT,	ARG_ARGUMENT_REQUIRED,	callback, .help="terminal width"},
		{'h', "height",		1, TYPE_INT,	ARG_ARGUMENT_REQUIRED,	callback, .help="terminal height"},
		{'n', "no-colour",	0, TYPE_BOOL,	0,			callback, .help="colourless"}
	};
	int options_count = sizeof(options) / sizeof(options[0]);

	args_setup(ARG_NONE_REQUIRED | ALLOW_INCORRECT_TYPES);
	args_set_help_character('H');
	args_load_spec(spec);
	args_parse(argc, argv, options_count, options, termspec);
	args_unsetup();
}

void terminal_scroll(terminal_spec_t * terminal) {
	uint16_t * text = terminal->text;
	for (int y = 1; y < terminal->cheight; y++) {
		for (int x = 0; x < terminal->cwidth; x++) {
			text[((y - 1) * terminal->cwidth) + x] = text[(y * terminal->cwidth) + x];
		}
	}
	int y = terminal->cheight - 1;
	for (int x = 0; x < terminal->cwidth; x++) {
		text[(y * terminal->cwidth) + x] = u'\0';
	}
	terminal->redraw_needed = 1;
}

void terminal_fixup(terminal_spec_t * terminal) {
	if (terminal->x < 0) {
		terminal->x = terminal->cwidth - 1;
		terminal->y--;
	}
	if (terminal->x == terminal->cwidth) {
		terminal->x = 0;
		terminal->y++;
	}
	if (terminal->y == terminal->cheight) {
		terminal_scroll(terminal);
		terminal->y--;
	}
}

uint16_t * terminal_get_character(terminal_spec_t * terminal, int x, int y) {
	return &terminal->text[(y * terminal->cwidth) + x];
}

void terminal_event_handler(event_t * event, void * priv) {
	if (event->type != EVENT_KEYBOARD) {
		return;
	}
	kbd_event_t * kbd_event = (kbd_event_t *) event;
	if (!kbd_event->pressed) {
		return;
	}
	uint16_t character = event_to_char(kbd_event, -1);
	terminal_handle_input(character, priv, 1);
}

int terminal_handle_escapes(uint16_t * chrp, terminal_spec_t * terminal, int input) {
	uint16_t chr = *chrp;
	switch (chr) {
		case u'\t':
			*chrp = u' ';
			return 0;
		case u'\0':
			return 1;
		case u'\n':
			terminal->y++;
			terminal->x = 0;
			terminal_fixup(terminal);
			if (input) {
				terminal_process_input(terminal);
			}
			return 1;
		case u'\r':
			terminal->x = 0;
			terminal_fixup(terminal);
			return 1;
		case u'\b':
			if (terminal->x == 0 && terminal->y == 0) {
				return 1;
			}
			if (input) {
				if (terminal_unwrite_input(terminal)) {
					return 1;
				}
			}
			uint16_t * c = terminal_get_character(terminal, --terminal->x, terminal->y);
			*c = u'\0';

			terminal_fixup(terminal);
			terminal->redraw_needed = 1;
			return 1;
	}
	return 0;
}

void terminal_handle_input(uint16_t chr, terminal_spec_t * terminal, int input) {
	if (terminal_handle_escapes(&chr, terminal, input)) {
		return;
	}
	uint16_t * c = terminal_get_character(terminal, terminal->x++, terminal->y);
	*c = chr;
	if (input) {
		terminal_write_input(&chr, 1, terminal);
	}

	terminal_fixup(terminal);
	terminal->redraw_needed = 1;
}

void terminal_writel(uint16_t * str, int size, terminal_spec_t * terminal) {
	while (size--) {
		terminal_handle_input(*str++, terminal, 0);
	}
}

void terminal_write(uint16_t * str, terminal_spec_t * terminal) {
	terminal_writel(str, ustrlen(str), terminal);
}

int terminal_unwrite_input(terminal_spec_t * terminal) {
	terminal_input_t * input = &terminal->input;
	if (input->items == 0) {
		return 1;
	}
	input->items--;
	return 0;
}

void terminal_write_input(uint16_t * str, int size, terminal_spec_t * terminal) {
	terminal_input_t * input = &terminal->input;
	if ((input->items + size) > input->capacity) {
		uint16_t * buffer = malloc((input->capacity + size + 33) * 2);
		memcpy(buffer, input->buffer, input->items * 2);
		free(input->buffer);
		input->buffer = buffer;
		input->capacity += size + 32;
	}
	memcpy(input->buffer + input->items, str, size * 2);
	input->items += size;
}

void terminal_write_handler(ansi_state_t * state, void * buffer, size_t size) {
	terminal_spec_t * terminal = state->priv;
	terminal_writel(buffer, size, terminal);
}

void terminal_exec(char * name, terminal_spec_t * terminal) {
	char first = *name;
	if (first == '#' || first == '\0') {
		terminal_prompt(terminal);
		return;
	}
	if (strcmp(name, "exit") == 0) {
		terminal->exit_needed = 1;
		terminal->redraw_needed = 1;
		return;
	}


	int argc = 0;
	char ** argv = lexer_parse(name, &argc);
	if (argc == 0) {
		terminal_prompt(terminal);
		return;
	}
	int pid = lctl(LCTL_INITRD_LOAD, argv[0], argc, argv);
	if (pid == 0) {
		char * name = argv[0];
		while (*name) {
			uint16_t c = *name++;
			terminal_writel(&c, 1, terminal);
		}
		terminal_write(u": not found.\n", terminal);
		terminal_prompt(terminal);
		return;
	}
	terminal->waitfor = pid;


	process_t * process = (process_t *) lctl(LCTL_GET_PCB, pid);

	//volatile uint32_t handler = (uint32_t) ansi_handler;
	//volatile uint32_t constructor = (uint32_t) ansi_constructor;
	//printf("%r\n", terminal_write_handler);
	//prctl(PR_HOOK_STDOUT, pid, 0, handler, constructor);
	//ansi_set_priv(process, terminal);
	//ansi_set_writer(process, (void *) write_handler);

	process->killed = 0;
}

void terminal_process_input(terminal_spec_t * terminal) {
	terminal_input_t * input = &terminal->input;
	input->buffer[input->items] = 0;

	char * name = malloc(input->items + 1);
	ustrtoa(input->buffer, name);
	terminal_exec(name, terminal);
}

void terminal_prompt(terminal_spec_t * terminal) {
	terminal_write(u"$ ", terminal);

	free(terminal->input.buffer);
	terminal->input.buffer = malloc(33 * 2);
	terminal->input.items = 0;
	terminal->input.capacity = 32;
}

int main(int argc, char * argv[]) {
	if (!__is_lemonos) {
		printf("this program only runs on LemonOS.\n");
		return -1;
	}

	terminal_spec_t spec = {420, 240, 1, 0xff000000, NULL, 0, 0};
	handle_argv(&spec, argc, argv);
	spec.cwidth = spec.width / 8;
	spec.cheight = spec.height / 16;
	spec.text = malloc((spec.cwidth * spec.cheight) * 2);
	spec.font = font_load();
	spec.input.buffer = NULL;
	memset(spec.text, 0, (spec.cwidth * spec.cheight) * 2);
	spec.waitfor = 0;
	spec.redraw_needed = 1;

	window_t * window = create_window(u"Terminal", u"terminal", spec.width, spec.height);
	window->priv = &spec;
	window->handle_event = terminal_event_handler;

	terminal_write(u"! WORK IN PROGRESS !\n", &spec);
	terminal_prompt(&spec);

	set_icon(window);
	while (1) {
		while (spec.redraw_needed != 1) {
			if (spec.waitfor) {
				if (!lctl(LCTL_GET_PCB, spec.waitfor)) {
					spec.waitfor = 0;
					terminal_prompt(&spec);
				}
			}
			sched_yield();
		}
		if (spec.exit_needed) {
			return 0;
		}
		render_frame(window, &spec, spec.text);
		spec.redraw_needed = 0;
	}
}
