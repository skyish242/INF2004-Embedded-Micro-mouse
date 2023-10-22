#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define LINE_THRESHOLD 1000
#define BUFFER_SIZE 9

uint32_t elapsed_times[BUFFER_SIZE];
int index = 0;

void timer_callback(struct repeating_timer *t){
    static uint32_t start_time = 0;

    uint16_t adc_value = adc_read();
    printf("%d\n", adc_value);

    if (adc_value >= LINE_THRESHOLD){
        printf("Line detected\n");
        if (start_time == 0) {
            start_time = time_us_32();
        }
    }
    else{
        if (start_time != 0){
            elapsed_times[index] = time_us_32() - start_time;
            start_time = 0;
            index++;
            
            if (index >= BUFFER_SIZE) {
                cancel_repeating_timer(t);
            }
        }
    }
}

int main(void) {
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);

    struct repeating_timer timer;

    add_repeating_timer_ms(100, timer_callback, NULL, &timer);

    while (index < BUFFER_SIZE) {
    }

    for (int i = 0; i < BUFFER_SIZE; i++) {
      printf("Elapsed Time %d: %u us\n", i, elapsed_times[i]);
    }

    return 0;
}
