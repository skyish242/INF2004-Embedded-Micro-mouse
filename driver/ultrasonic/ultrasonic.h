#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#include "motor.h"

const uint TRIG_PIN = 18;  // GPIO 18 for the TRIG pin
const uint ECHO_PIN = 19;  // GPIO 19 for the ECHO pin

static absolute_time_t start_time;
static absolute_time_t end_time;
static bool rising_edge = true;

void echo_pin_handler(uint gpio, uint32_t events);
double kalman(double U);
void setup();