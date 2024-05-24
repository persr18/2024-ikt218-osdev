#include <libc/stdint.h>
#include <io.h>

/// @brief Write a byte to a port
/// @param port 
/// @param val 
void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

/// @brief Read a byte from a port
/// @param port 
/// @return 
uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0"
                     : "=a"(ret)
                     : "Nd"(port)
                     : "memory");
    return ret;
}

/// @brief Disable interrupts
/// @param  
void cli(void)
{
    __asm__ volatile("cli");
}
