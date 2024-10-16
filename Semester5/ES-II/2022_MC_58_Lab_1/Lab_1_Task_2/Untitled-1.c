#include <stdio.h>

// Function to write a single character to the LCD (stub for demonstration)
void write_LCD_Char(char c) {
    printf("%c", c); // For demonstration, we print to the console
}

// Function to calculate the number of digits in an integer
int calculateLength(int number) {
    int length = 0;
    if (number == 0) return 1; // Special case for 0
    while (number != 0) {
        number /= 10;
        length++;
    }
    return length;
}

// Function to convert an integer to its individual digits and display on LCD
void write_LCD_number(int number) {
    int length = calculateLength(number);
    char digits[length + 1]; // Array to hold the individual digits as characters

    // Extract each digit from the end
    for (int i = length - 1; i >= 0; i--) {
        digits[i] = (number % 10) + '0'; // Convert digit to character
        number /= 10;
    }
    digits[length] = '\0'; // Null terminator

    // Write each digit to the LCD
    for (int i = 0; i < length; i++) {
        write_LCD_Char(digits[i]);
    }
}

int main() {
    int number = 5432;
    write_LCD_number(number); // This will print "5432" to the console

    return 0;
}
