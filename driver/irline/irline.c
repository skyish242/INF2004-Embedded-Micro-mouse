#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

int main(void) {
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);

    while (1) {
        uint16_t adc_value = adc_read();
        printf("%d\n", adc_value);
        // sleep_ms(1000);
    }
}