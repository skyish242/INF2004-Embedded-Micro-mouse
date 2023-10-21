#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"

const uint TRIG_PIN = 2;  // GPIO 2 for the TRIG pin
const uint ECHO_PIN = 3;  // GPIO 3 for the ECHO pin

static absolute_time_t start_time;
static absolute_time_t end_time;
static bool rising_edge = true;

void echo_pin_handler(uint gpio, uint32_t events) {
    if (rising_edge) {
        start_time = get_absolute_time();
        gpio_set_irq_enabled(ECHO_PIN, GPIO_IRQ_EDGE_FALL, true);
    } else {
        end_time = get_absolute_time();
        gpio_set_irq_enabled(ECHO_PIN, GPIO_IRQ_EDGE_RISE, true);
    }
    rising_edge = !rising_edge;
}

void setup() {
    stdio_init_all();
    gpio_init(TRIG_PIN);
    gpio_init(ECHO_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(ECHO_PIN, GPIO_IRQ_EDGE_RISE, true, &echo_pin_handler);
    gpio_set_irq_enabled_with_callback(ECHO_PIN, GPIO_IRQ_EDGE_FALL, true, &echo_pin_handler);
    gpio_put(TRIG_PIN, 0);
    sleep_ms(2);
}

int main() {
    setup();

    while (1) {
        gpio_put(TRIG_PIN, 1);
        sleep_us(10); // Send a 10us pulse on the TRIG pin
        gpio_put(TRIG_PIN, 0);

        // Wait for the measurement to complete (interrupts handle the timing)
        sleep_ms(100); // Adjust this based on your sensor's maximum range

        // Calculate distance based on the timestamps
        int32_t duration_us = absolute_time_diff_us(start_time, end_time);
        float distance_cm = duration_us / 58.0f; // Convert to centimeters (approximate)

        printf("Distance: %.2f cm\n", distance_cm);

        if (distance_cm <= 5.0) {
            printf("Warning: Object is within 5 cm!\n");
        }

        sleep_ms(500); // Delay before the next measurement
    }

    return 0;
}
