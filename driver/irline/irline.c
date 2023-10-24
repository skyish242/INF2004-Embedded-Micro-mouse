#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define LINE_THRESHOLD 1500
#define BUFFER_SIZE 10

uint32_t line_timings[BUFFER_SIZE];
uint32_t elapsed_times[BUFFER_SIZE - 1];
int timer_index = 0;

bool timer_callback(struct repeating_timer *t){
    static bool color_changed = true;

    uint16_t adc_value = adc_read();

    if (adc_value >= LINE_THRESHOLD){
        if (color_changed){
            color_changed = false;
            // line_timings[timer_index] = to_us_since_boot(get_absolute_time());
            line_timings[timer_index] = time_us_64();
            timer_index++;
        }
    }
    else{
        if (!color_changed){
            color_changed = true;
            line_timings[timer_index] = time_us_64();
            timer_index++;
            
        }
    }
    // if (timer_index >= BUFFER_SIZE) {
        // cancel_repeating_timer(t);
    // }
    return true;
}

void getElapsedTimes(uint32_t arr[]){
    for (int i = 0; i < BUFFER_SIZE - 1; i++){
        elapsed_times[i] = arr[i+1] - arr[i];
    }
}

void findTopThree(uint32_t arr[], int *max1, int *max2, int *max3){
    *max1 = 0;
    *max2 = 0;
    *max3 = 0;

    for(int i = 0; i < BUFFER_SIZE; i++){
        if (arr[i] > *max1){
          *max3 = *max2;
          *max2 = *max1;
          *max1 = arr[i];
        }
        else if (arr[i] > *max2){
           *max3 = *max2;
           *max2 = arr[i];
        }
        else if (arr[i] > *max3){
           *max3 = arr[i];
        }
    }
}

int main(void) {
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);
    int max1, max2, max3;

    struct repeating_timer timer;

    add_repeating_timer_ms(100, timer_callback, NULL, &timer);

    while (timer_index < BUFFER_SIZE) {
    }

    getElapsedTimes(line_timings);
    findTopThree(elapsed_times, &max1, &max2, &max3);

    printf("%d\n", max1);
    printf("%d\n", max2);
    printf("%d\n", max3);

    for (int i = 0; i < BUFFER_SIZE - 1; i++) {
        printf("Elapsed Time %d: %lu us\n", i, elapsed_times[i]);
    }

    return 0;
}
