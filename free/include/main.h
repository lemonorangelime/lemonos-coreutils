#pragma once

typedef struct {
	uint8_t human : 1;
	uint8_t si : 1;
	uint8_t lohi : 1;
	uint8_t total : 1;
	uint8_t wide : 1;
	int format;
	sysinfo_t * info;
} mypriv_t;

enum {
	BYTES,
	KILO,
	MEGA,
	GIGA,
	TERA,
	PETA,
};
