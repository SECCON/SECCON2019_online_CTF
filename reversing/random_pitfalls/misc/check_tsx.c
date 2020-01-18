#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int n)
{
	printf("TSX is NOT supported.(signum=%d)\n", n);
	exit(-1);
}


int main()
{
	signal(SIGSEGV, handler);
	signal(SIGILL, handler);
	asm volatile (
		"xbegin 1f\n"
		"xor %rax, %rax\n"
		"mov %rax, (%rax)\n"
		"xend\n"
		"1:\n"
	);
	puts("TSX is supported.");
	return 0;
}
