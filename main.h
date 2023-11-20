#include <stdio.h>
#include <string.h>
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"
#include "queue.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"


#include "magnometer.h"
// #include "web_server.h"
#include "irline.h"
#include "floodfill.h"

static void printMessage(const char* message);
static void vPrintTask(void* pvParameters);
static void vDirectionTask(void* pvParameters);
static void vLineTask(void* pvParameters);
static void vFloodfillTask(void* pvParameters);