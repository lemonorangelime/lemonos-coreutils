bits 32

extern rdmxcsr
rdmxcsr:
	sub esp, 4
	stmxcsr DWORD [esp]
	pop eax
	ret

extern rdfsw
rdfsw:
	xor eax, eax
	fstsw ax
	ret
