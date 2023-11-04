#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"

const uint TRIG_PIN = 18;  // GPIO 2 for the TRIG pin
const uint ECHO_PIN = 19;  // GPIO 3 for the ECHO pin

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

double kalman(double U) {
    static const double R = 40;
    static const double H = 1;
    static double Q = 10;
    static double P = 0;
    static double U_hat = 0;
    static double K = 0;
    K = P * H / (H * P * H + R);
    U_hat += K * (U - H * U_hat);
    P = (1 - K * H) * P + Q;
    return U_hat;
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

    double filtered_distance_cm = 0; // Initialize the filtered distance
    while (1) {
        gpio_put(TRIG_PIN, 1);
        sleep_us(10); // Send a 10us pulse on the TRIG pin
        gpio_put(TRIG_PIN, 0);

        // Wait for the measurement to complete (interrupts handle the timing)
        float max_distance_cm = 5;  // Maximum expected distance in centimeters
        float speed_of_sound_cm_per_s = 34300;  // Speed of sound in centimeters per second

        // Calculate the delay in milliseconds based on the maximum distance
        int delay_ms = (2 * max_distance_cm / speed_of_sound_cm_per_s) * 1000;
        sleep_ms(delay_ms); // Adjust this based on your sensor's maximum range

        // Calculate distance based on the timestamps
        int32_t duration_us = absolute_time_diff_us(start_time, end_time);
        float distance_cm = duration_us / 58.0f; // Convert to centimeters (approximate)

        // Use the Kalman filter to estimate the distance
        filtered_distance_cm = kalman(distance_cm);

        printf("(Distance: %.2f cm)\n",filtered_distance_cm);

        if (filtered_distance_cm <= 5.0) {
            printf("Warning: Object is within 5 cm!\n");
        }

        // Delay before the next measurement (10 times per second update rate)
        sleep_ms(100);

    }

    return 0;
}
