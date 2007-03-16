section .text
global clock64:function
clock64:
	push ebx
	cpuid
	rdtsc
	pop ebx
	RET
