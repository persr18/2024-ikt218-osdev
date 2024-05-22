#include "terminal.h"
#include "libc/stdint.h"
#include "libc/stdarg.h"
#include "libc/stdbool.h"

// Setting current position of cursor.
int col, row = 0;

// Creating a pointer to the start of the video memory.
uint16_t *const video = (uint16_t *const)0xB8000;

// Defining default color by combining background and text color values.
uint16_t const defaultColor = (BLACK << 12) | (GRAY << 8);

// Setting selected color to default color.
uint16_t selectedColor = defaultColor;

void monitorInitialize()
{
}

// Function to set color.
// Ensures that color codes used are within valid range.

/// @brief takes in two color values and sets the background and text color to these values.
/// @param background
/// @param text
void setColors(uint16_t background, uint16_t text)
{
    // Check if color codes are within valid range.
    if (background > 0xF)
    {
        terminalWrite("Background color out of range\n");
        // If not, return.
        return;
    }
    // Check if color codes are within valid range.
    if (text > 0xF)
    {
        terminalWrite("Text color out of range\n");
        // If not, return.
        return;
    }
    // Set selected color to the combined color values.
    selectedColor = (background << 12) | (text << 8);
}

void disable_cursor()
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

/// @brief moves the cursor to the specified position.
/// @param x
/// @param y
void move_cursor(int x, int y)
{
    // Calculate the position of the cursor.
    uint16_t pos = y * width + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

/// @brief writes a string to the screen.
/// @param string
void terminalWrite(const char *string)
{
    // Create a pointer to the start of the string.
    char *ptr = string;

    // Loop through the string.
    while (*ptr != '\0')
    {
        // Check if the character is a newline character.
        if (*ptr == '\n')
        {
            col = 0;
            row++;
            if (row == height)
            {
                scrollUp();
            }
            *ptr++;
            continue;
        }
        // Check if the character is a carriage return character.
        if (*ptr == '\r')
        {
            // Move the cursor to the start of the row.
            col = 0;
            *ptr++;
            continue;
        }
        // Write the character to the screen at the current position.
        video[row * width + col] = selectedColor | *ptr;
        *ptr++;
        col++;
    }
    // move the cursor to the new position.
    move_cursor(col, row);
    return;
}

// Function to clear screen. Basically writes blank space to every field on the grid/screen.
void clearScreen()
{
    // Set row and col to top left corner.
    row = 0;
    col = 0;

    // Loop through the screen.
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Write blank space to the current position.
            video[y * width + x] = ' ' | defaultColor;
        }
    }
    // Move cursor to top left corner.
    move_cursor(col, row);
}

// Function to scroll text up. Copies every character up one row and writes spaces to the bottom row.
void scrollUp()
{
    row--;
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width; x++)
        {

            video[y * width + x] = video[(y + 1) * width + x];
        }
    }

    for (int x = 0; x < width; x++)
    {
        video[(height - 1) * width + x] = ' ' | defaultColor;
    }

    move_cursor(col, row);
}

// itoa() implementation from https://www.geeksforgeeks.org/implement-itoa/
char *itoa(int num, char *str, int base)
{
    int i = 0;
    bool isNegative = false;

    // Print 0 as value
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Check if number is negative
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }

    return str;
}

/// @brief printf function that takes in a string and a variable number of arguments.
/// @param str
/// @param
void printf(const char *str, ...)
{
    va_list ptr;
    va_start(ptr, str);

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != '%')
        {
            char Chars[2] = {str[i], '\0'};
            terminalWrite(Chars);
        }
        else if (str[i] == '%' && (str[i + 1] == 'i' || str[i + 1] == 'd'))
        {
            char digits[11];
            itoa(va_arg(ptr, int), digits, 10);
            terminalWrite(digits);
            i++;
        }
        else if (str[i] == '%' && str[i + 1] == 's')
        {
            char *s = va_arg(ptr, char *);
            terminalWrite(s);
            i++;
        }
        else if (str[i] == '%' && str[i + 1] == 'x')
        {
            char digits[9];
            itoa(va_arg(ptr, int), digits, 16);
            terminalWrite(digits);
            i++;
        }
        else
        {
            break;
        }
    }
    va_end(ptr);
}

void drawCake()
{
    setColors(BLACK, BLACK);
    printf("         ");
    setColors(WHITE, BLACK);
    printf(" ");
    setColors(BLACK, BLACK);
    printf("  ");
    setColors(WHITE, BLACK);
    printf(" ");
    printf("\n");

    setColors(BLACK, BLACK);
    printf("       ");
    setColors(GRAY, BLACK);
    printf("        ");
    printf("\n");

    setColors(BLACK, BLACK);
    printf("     ");
    setColors(PURPLE, BLACK);
    printf("            ");
    printf("\n");

    setColors(BLACK, BLACK);
    printf("   ");
    setColors(LIGHT_RED, BLACK);
    printf("                ");
    printf("\n");

    setColors(BLACK, BLACK);
    printf(" ");
    setColors(YELLOW, BLACK);
    printf("                    ");
    printf("\n");
    setColors(BLACK, GRAY);
}