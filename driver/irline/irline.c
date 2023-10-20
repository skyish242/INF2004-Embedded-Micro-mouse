#include <stdio.h>
#include "pico/stdlib.h"

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 16
#define UART_RX_PIN 17

const uint BTN_PIN = 15;

int main() {
    char charReceiveData, charSendData, charPrintData;
    stdio_usb_init();

    // Initialize the UART communication through the feedback loop
    uart_init(UART_ID, BAUD_RATE);

    // Setting the direction of the pseudo button GPIO pin
    gpio_set_dir(BTN_PIN, GPIO_IN);

    // Using a Pull-down Active-High method to configure the "pseudo" button
    gpio_set_pulls(BTN_PIN, true, false);

    // Set the GPIO pins 16 and 17 to the UART function
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    charSendData = 'A';
    while (true) { 
        // Checks if pseudo button is being pressed (GPIO High), and then it will send '1' over UART
        if(gpio_get(BTN_PIN)) {
            uart_putc(UART_ID, '1');
        }

        else {
            // If the pseudo button is not pressed (GPIO low), send characters 'A' to 'Z' sequentially
            uart_putc(UART_ID, charSendData++);

            // Check if the character has reached 'Z' and reset it to 'A'
            if (charSendData > 'Z') {
                charSendData = 'A';
            }
        }

        if(uart_is_readable(UART_ID)){
            // If UART has received data
            charReceiveData = uart_getc(UART_ID);

            if(charReceiveData == '1') {
                // If '1' is received, increment it to '2'
                charPrintData = ++charReceiveData;
            }

            else {
                // If any other character is received, convert to lowercase by adding 32
                charPrintData = charReceiveData + 32;
            }
            // Print the data that has been modified to suit requirements
            printf("%c\n", charPrintData);
        }

        sleep_ms(1000); // Sleep for 1 second to suit requirements
    }
    return 0;
}
