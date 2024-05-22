// The implementatio of interrups is inspired by different sources, and tutorials from OSDev

#pragma once
#include "libc/stdint.h"


// The function idt_entry represents the gate descriptor interrupt handling
struct idt_entry {
    uint16_t offset_1;       // First 16 bits
    uint16_t selector;       // The selector points to a valid address in GDT
    uint8_t zero;            // Reserved byte for the IDT to function
    uint8_t type_attributes; // Attributes used for the IDT
    uint16_t offset_2;       // Last 16 bits
} __attribute__((packed));

// The function idt_pointer is used for ensuring the IDT loaded into the IDT register starts at correct address and has correct size
struct idt_pointer {
    uint16_t size;           // Size of the IDT
    uint32_t base;           // Address of the IDT
} __attribute__((packed));

void initIdt();
void createIdtEntry(int index, void *offset, uint8_t attributes);
void remapPIC();