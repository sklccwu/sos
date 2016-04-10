#include "vmm.h"
#include "debug.h"
#include "idt.h"
#include "scrn.h"

void page_fault(struct regs *regs)
{
	uint32_t cr2;
	asm volatile ("mov %%cr2, %0" : "=r" (cr2));

	printf("Page fault at 0x%x, virtual faulting address 0x%x\n", regs->eip, cr2);
	printf("Error code: %x\n", regs->err_code);

	// bit 0 为 0 指页面不存在内存里
	if ( !(regs->err_code & 0x1)) {
		printf("Because the page wasn't present.\n");
	}
	// bit 1 为 0 表示读错误，为 1 为写错误
	if (regs->err_code & 0x2) {
		printf("Write error.\n");
	} else {
		printf("Read error.\n");
	}
	// bit 2 为 1 表示在用户模式打断的，为 0 是在内核模式打断的
	if (regs->err_code & 0x4) {
		printf("In user mode.\n");
	} else {
		printf("In kernel mode.\n");
	}
	// bit 3 为 1 表示错误是由保留位覆盖造成的
	if (regs->err_code & 0x8) {
		printf("Reserved bits being overwritten.\n");
	}
	// bit 4 为 1 表示错误发生在取指令的时候
	if (regs->err_code & 0x10) {
		printf("The fault occurred during an instruction fetch.\n");
	}

	while (1);
}

