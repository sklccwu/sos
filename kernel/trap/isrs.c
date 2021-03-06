#include "isrs.h"
#include "idt.h"
#include "scrn.h"

static const char * trapname(int trapno);

static void *isr_routines[32] = 
{
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};

/* This is a very repetitive function... it's not hard, it's
*  just annoying. As you can see, we set the first 32 entries
*  in the IDT to the first 32 ISRs. We can't use a for loop
*  for this, because there is no way to get the function names
*  that correspond to that given entry. We set the access
*  flags to 0x8E. This means that the entry is present, is
*  running in ring 0 (kernel level), and has the lower 5 bits
*  set to the required '14', which is represented by 'E' in
*  hex. */
void isrs_install()
{
    idt_set_gate(0, (unsigned)isr0, 0x08, 0x8E);
    idt_set_gate(1, (unsigned)isr1, 0x08, 0x8E);
    idt_set_gate(2, (unsigned)isr2, 0x08, 0x8E);
    idt_set_gate(3, (unsigned)isr3, 0x08, 0x8E);
    idt_set_gate(4, (unsigned)isr4, 0x08, 0x8E);
    idt_set_gate(5, (unsigned)isr5, 0x08, 0x8E);
    idt_set_gate(6, (unsigned)isr6, 0x08, 0x8E);
    idt_set_gate(7, (unsigned)isr7, 0x08, 0x8E);

    idt_set_gate(8, (unsigned)isr8, 0x08, 0x8E);
    idt_set_gate(9, (unsigned)isr9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned)isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned)isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned)isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned)isr15, 0x08, 0x8E);

    idt_set_gate(16, (unsigned)isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned)isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned)isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned)isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned)isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned)isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned)isr23, 0x08, 0x8E);

    idt_set_gate(24, (unsigned)isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned)isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned)isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned)isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned)isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned)isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned)isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned)isr31, 0x08, 0xEE);
    
}

void isr_install_handler(int isr,void (*handler)(struct regs * r))
{
	isr_routines[isr] = handler;
}

void isr_uninstall_handler(int isr)
{
	isr_routines[isr] = 0;
}

/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
void isr_handler(struct regs *r)
{
    void (*handler)(struct regs *r);
    handler = isr_routines[r->int_no];
    if(handler)
    {
        handler(r);
    }
    else if (r->int_no < 32)
    {
        printf("%s",trapname(r->int_no));
        printf(" Exception. System Halted!\n");
        for (;;);
    }
}

static const char * trapname(int trapno) 
{
    static const char * const excnames[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
    };

    if (trapno < sizeof(excnames)/sizeof(const char * const)) {
        return excnames[trapno];
    }
    return "(unknown trap)";
}

//void print_cur_status(void) {
//    static int round = 0;
//    uint16_t reg1, reg2, reg3, reg4;
//    asm volatile (
//            "mov %%cs, %0;"
//            "mov %%ds, %1;"
//            "mov %%es, %2;"
//            "mov %%ss, %3;"
//            : "=m"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4));
//    Printf("%d:  @ring %d\n", round, reg1 & 3);
//    Printf("%d:  cs = %x\n", round, reg1);
//    Printf("%d:  ds = %x\n", round, reg2);
//    Printf("%d:  es = %x\n", round, reg3);
//    Printf("%d:  ss = %x\n", round, reg4);
//    round ++;
//}
//
//void switch_to_user(void) {
//	asm volatile (
//	    "sub $0x8, %%esp \n"
//	    "int %0 \n"
//	    "movl %%ebp, %%esp"
//	    : 
//	    : "i"(T_SWITCH_TOU)
//	);
//}
//
//void switch_to_kernel(void) {
//	asm volatile (
//	    "int %0 \n"
//	    "movl %%ebp, %%esp \n"
//	    : 
//	    : "i"(T_SWITCH_TOK)
//	);
//}
//void print_regs(struct regs *r)
//{
//	Printf("r at %x\n",&r);
//	Printf("cs : %x\n",r->cs);
//	Printf("ds : %x\n",r->ds);
//	Printf("es : %x\n",r->es);
//	Printf("ss : %x\n",r->ss);
//	Printf("int_no: %d\n",r->int_no);
//	Printf("esp : %x\n\n",r->useresp);
//}
//void switch_test()
//{
//	print_cur_status();
//	puts("switch to user mode.\n");
//	switch_to_user();
//	print_cur_status();
//	puts("switch to kernel mode.\n");
//	switch_to_kernel();
//	print_cur_status();
//}
//struct regs switchk2u,*switchu2k;
//void switch_tou_handler(struct regs *r)
//{
//	//puts("\nswitch_tou_handler install ready\n");
//	//print_regs(r);
//	if(r->cs != 0x1B)
//	{
//		switchk2u = *r;
//		switchk2u.cs = 0x1B;
//		switchk2u.ds = switchk2u.es = switchk2u.ss = 0x23;
//		switchk2u.useresp = (uint32_t)r + sizeof(struct regs) - 8;
//            switchk2u.eflags |= 0x00003000;
//            *((uint32_t *)r- 1) = (uint32_t)&switchk2u;
//	 //   print_regs(&switchk2u);
//	}
//	//print_regs(r);
//}
//void switch_tok_handler(struct regs *r)
//{
//	//puts("switch_tok_handler install ready\n");
//	if(r->cs != 0x08)
//	{
//		r->cs = 0x08;
//		r->ds = r->es = 0x10;
//		r->eflags &= ~(0x00003000);
//		switchu2k = (struct regs *)(r->useresp - (sizeof(struct regs) - 8));
//		memcpy(switchu2k, r, sizeof(struct regs) - 8);
//		*((uint32_t *)r - 1) = (uint32_t)switchu2k;
//	}
//}
//void switch_install()
//{
//	isr_install_handler(30,switch_tou_handler);
//	isr_install_handler(31,switch_tok_handler);
//}
