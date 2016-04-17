; Loads the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void idt_load();'
bits 32
section .text
global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret