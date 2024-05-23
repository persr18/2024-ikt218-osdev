#include <multiboot2.h>
#include <gdt.h>
#include <idt.h>
#include <terminal.h>
#include <io.h>
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>
#include <isr.h>
#include <irq.h>
#include <memory.h>
#include <pit.h>

extern void interrupt();
extern uint32_t end;

struct multiboot_info
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();

int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{

//startup
    printf("Initializing\n\n");
    gdtInit();
    printf("GDT initialized\n");
    initIdt();
    printf("IDT initialized\n");
    initIrq();
    init_paging();
    initPit();
    printf("PIT initialized with target frequency of %d Hz\n\n", TARGET_FREQUENCY);
    sleep_interrupt(3000);
    clearScreen();
    init_kernel_memory(&end);
    sleep_interrupt(3000);


// TESTING PIT
   /* int counter = 0;
    while (true)
    {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    }; 
    */

// TESTING PRINTING
/*
    clearScreen();
    printf("Hello World from printf \n");
    terminalWrite("Hello World from terminalWrite");
    sleep_busy(5000);
*/

    return kernel_main();
}