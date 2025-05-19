#include <stdio.h>
#include <fpu.h>
#include <cpu.h>

int main(int argc, char * argv[]) {
	printf("MXCSR: %r\n", rdmxcsr());
	printf("FSW: %r\n", rdfsw());

	printf("\n");
	printf("cr0: %r\n", rdcr0());
	printf("cr1: %r\n", rdcr1());
	printf("cr2: %r\n", rdcr2());
	printf("cr3: %r\n", rdcr3());
	printf("cr4: %r\n", rdcr4());
	printf("cr5: %r\n", rdcr5());
	printf("cr6: %r\n", rdcr6());
	printf("cr7: %r\n", rdcr7());
	return 0;
}
