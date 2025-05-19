#include <stdio.h>
#include <time.h>

int log10i(int x) {
    int r = 0;
    while (x >= 10) {
        x /= 10;
        r++;
    }
    return r;
}


int my_log10i(int x) {
	static uint32_t table[] = {9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999};
	int y = (9 * (31 - __builtin_clz(x | 1))) >> 5;
	y += x > table[y];
	return y + 1;
}

float my_blormfise(int x, int y) {
	unsigned int pow10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	int a = x * pow10[my_log10i(y)] + y;
	int l = my_log10i(a) - 1;

	int A = a;
	int f = 0;
	while (A != 0) {
		f = f * 10 + (A % 10);
		A /= 10;
	}

	int b = f % 10 * pow10[l] + f / 10;

	int r = 0;
	for (int i = 0; i <= l; i++) {
		r += pow10[i] * ((a % 10 - b % 10 + 10) % 10);
		a /= 10;
		b /= 10;
	}
	return (float) r / (x + y);
}

float blormfise(int x, int y) {
	unsigned int pow10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	// x = ABC, y = DEF
	// a = ABCDEF
	int a = x * pow10[log10i(y) + 1] + y;
	int l = log10i(a);

	// f = FEDCBA
	int A = a;
	int f = 0;
	int d = 0;
	while (A != 0) {
		d = A % 10;
		f = f * 10 + d;
		A /= 10;
	}

	// b = AFEDCB
	int b = f % 10 * pow10[l] + f / 10;

	int r = 0;
	for (int i = 0; i <= l; i++) {
		r += pow10[i] * ((a % 10 - b % 10 + 10) % 10);
		a /= 10;
		b /= 10;
	}
	return (float)r / (x + y);
}

int test() {
	int i = 25000000;
	float r = 0;
	uint64_t start1 = get_timer_timestamp();
	while (i--) {
		r += blormfise(1000, 1000);
		r += blormfise(1000, 1000);
		r += blormfise(1000, 1000);
		r += blormfise(1000, 1000);
	}
	uint64_t end1 = get_timer_timestamp();

	printf("blormfise(10000, 1000): (%d ms)\n", end1-start1);

	i = 25000000;
	float r2 = 0;
	uint64_t start2 = get_timer_timestamp();
	while (i--) {
		r2 += my_blormfise(1000, 1000);
		r2 += my_blormfise(1000, 1000);
		r2 += my_blormfise(1000, 1000);
		r2 += my_blormfise(1000, 1000);
	}
	uint64_t end2 = get_timer_timestamp();

	if (r != r2) {
		printf("FAIL\n");
	}

	printf("my_blormfise(10000, 1000): (%d ms)\n", end2-start2);

	return (end1-start1) - (end2-start2);
}

int main(int argc, char * argv[]) {
	int i = 0;
	int r = 0;
	while (i < 10) {
		int result = test();
		r += result;
		printf("result: %c%d ms\n", (result > 0) ? '+' : '\0', result);
		i++;
	}
	r /= 10;
	printf("Average: %c%d ms\n", (r > 0) ? '+' : '\0', r);
}
