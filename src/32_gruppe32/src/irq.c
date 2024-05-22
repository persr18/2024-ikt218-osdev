// The implementatio of interrups is inspired by the guide from James Molloy, and tutorials from OSDev

#include <isr.h>
#include <io.h>
#include <terminal.h>
#include <pit.h>
#include <keyboard.h>

void initIrq()
{
    set_isr_handler(IRQ0, &pit_handler);
    set_isr_handler(IRQ1, &keyboard_handler);
}

void irq_handler(registers_t r) {
    if (r.int_no >= 40) {
        
        outb(PIC_SEC_CONTROL, 0x20);
    }
  
    outb(PIC_MAIN_CONTROL, 0x20);
  
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
    else {
        printf("Interrupt received: ");
        printf("%d", r.err_code);
        printf("\n");

        printf(exception_messages[r.int_no]);
        printf("\n");
    }
}