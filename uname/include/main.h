#pragma once

#include <stdint.h>

// non-performance sensitive
typedef struct {
	uint32_t all : 1;
	uint32_t kernel_name : 1;
	uint32_t nodename : 1;
	uint32_t kernel_release : 1;
	uint32_t kernel_version : 1;
	uint32_t machine : 1;
	uint32_t processor : 1;
	uint32_t hardware_platform : 1;
	uint32_t operating_system : 1;
	uint32_t help : 1;
	uint32_t version : 1;
} uname_bits_t;

typedef struct {
	union {
		uname_bits_t bits;
		uint32_t all_bits;
	};
} uname_state_t;
