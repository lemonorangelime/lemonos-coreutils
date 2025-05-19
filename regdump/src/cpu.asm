bits 32

extern rdcr0
rdcr0:
	mov eax, cr0
	ret

extern rdcr1
rdcr1:
	mov eax, cr1
	ret

extern rdcr2
rdcr2:
	mov eax, cr2
	ret

extern rdcr3
rdcr3:
	mov eax, cr3
	ret

extern rdcr4
rdcr4:
	mov eax, cr4
	ret

extern rdcr5
rdcr5:
	mov eax, cr5
	ret

extern rdcr6
rdcr6:
	mov eax, cr6
	ret

extern rdcr7
rdcr7:
	mov eax, cr7
	ret
