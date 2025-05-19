#pragma once

#include <stdint.h>
#include <widgets.h>

typedef struct {
	int inside;
	int clicking;
	uint32_t colour;
	int prev_x, prev_y;
	int x, y;
	int first;
	rect_2d_t * rect;
	widgets_element_t * hitbox;
} priv_t;
