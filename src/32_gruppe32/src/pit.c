#include <libc/stdint.h>
#include <libc/limits.h>
#include <isr.h>
#include <pit.h>
#include <io.h>
#include <terminal.h>

void initPit() {
    uint16_t divisor = DIVIDER;
    outb(PIT_CMD_PORT, 0x36);
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);
}

uint32_t ticks = 0;

void pit_handler(registers_t r) {
    if (ticks == UINT32_MAX)
        ticks = 0;
    ticks++;
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start_ticks = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;
    while (elapsed_ticks < ticks_to_wait) {
        while (ticks == elapsed_ticks + start_ticks) {
            // Busy wait
        }
        elapsed_ticks++;
    }
}

void sleep_interrupt(uint32_t milliseconds){
    uint32_t current_tick = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = current_tick + ticks_to_wait;

    while (current_tick < end_ticks) {
        asm volatile("sti"); // Enable interrupts (sti)
        asm volatile("hlt"); // Halt the CPU until the next interrupt (hlt)
        current_tick = ticks;
    }
}