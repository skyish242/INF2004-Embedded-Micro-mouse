#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define LINE_THRESHOLD 1500
#define BUFFER_SIZE 10

typedef struct {
    int binary[9];
    char character;
} Code39Mapping;

void getElapsedTimes(uint32_t arr[]);

void findTopThree(uint32_t arr[], int *max1, int *max2, int *max3);

char decodeCode39(int binaryArray[9]);

void reverseBinaryArray(int binaryArray[9], int reversedArray[9]);

bool timer_callback(struct repeating_timer *t);

int detectLine(int adc_input);

void ir_init();