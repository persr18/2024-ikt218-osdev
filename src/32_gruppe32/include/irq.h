// The implementatio of interrups is inspired by different sources, and tutorials from OSDev

#pragma once
#include <libc/stdint.h>
#include <isr.h>

void initIrq();
void irq_handler(registers_t r);