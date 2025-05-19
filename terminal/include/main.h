#pragma once

#include <stdint.h>
#include <font.h>

typedef struct {
	uint16_t * buffer;
	int items;
	int capacity;
} terminal_input_t;

typedef struct {
	int width;
	int height;
	int has_colour;
	uint32_t background_colour;
	uint16_t * text;
	int x;
	int y;
	int cwidth;
	int cheight;
	font_t * font;
	int redraw_needed;
	int exit_needed;
	terminal_input_t input;
	int waitfor;
} terminal_spec_t;

void terminal_handle_input(uint16_t chr, terminal_spec_t * spec, int input);
void terminal_process_input(terminal_spec_t * terminal);
void terminal_write(uint16_t * str, terminal_spec_t * terminal);
int terminal_unwrite_input(terminal_spec_t * terminal);
void terminal_write_input(uint16_t * str, int size, terminal_spec_t * terminal);
void terminal_prompt(terminal_spec_t * terminal);
