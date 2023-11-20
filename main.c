#include "main.h"

#ifndef RUN_FREERTOS_ON_CORE
#define RUN_FREERTOS_ON_CORE 0
#endif

#define mbaTASK_MESSAGE_BUFFER_SIZE (60)

#define TEST_TASK_PRIORITY (tskIDLE_PRIORITY + 1UL )
#define PRINT_TASK_PRIORITY (tskIDLE_PRIORITY + 8UL)
#define DEFAULT_TASK_PRIORITY (tskIDLE_PRIORITY + 8UL)

// Configuration Parameters
#define LARGE_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define MESSAGE_BUFFER_SIZE 64
#define QUEUE_LENGTH 10
#define MAX_MESSAGE_LENGTH 54

static QueueHandle_t xMessageQueue;

int main(){
    //stdio_usb_init();
    stdio_init_all();

    // Initialize LSM303DLH accelerometer and magnetometer (accelerometer not printed in this code but is defined, if needed in future)
    lsm303dlh_acc_setup();
    lsm303dlh_mag_setup();

    // Initialize IR Sensors
    adc_init();
    adc_gpio_init(26);

    // xMessageQueue = xQueueCreate(QUEUE_LENGTH, MAX_MESSAGE_LENGTH * sizeof(char));

    // xTaskCreate(vDirectionTask, "DirectionTask", configMINIMAL_STACK_SIZE, NULL, DEFAULT_TASK_PRIORITY, NULL);
    // xTaskCreate(vLineTask, "LineTask", configMINIMAL_STACK_SIZE, NULL, DEFAULT_TASK_PRIORITY, NULL);
    xTaskCreate(vFloodfillTask, "FloodfillTask", configMINIMAL_STACK_SIZE, NULL, DEFAULT_TASK_PRIORITY, NULL);
    // xTaskCreate(vPrintTask, "PrintTask", configMINIMAL_STACK_SIZE, NULL, PRINT_TASK_PRIORITY, NULL);

    // Start FreeRTOS scheduler
    vTaskStartScheduler();

    // The scheduler should never return, but if it does, an error occurred
    while (1) {
        printf("Error: FreeRTOS scheduler returned.\n");
    }

    return 0;

}

// static void printMessage(const char* message)
// {
//     printf("%s\n", message);
// }

// static void vPrintTask(void* pvParameters)
// {
//     char receivedMessage[MAX_MESSAGE_LENGTH];
    
//     while (1)
//     {
//         if (xQueueReceive(xMessageQueue, receivedMessage, portMAX_DELAY) == pdPASS)
//         {
//             printMessage(receivedMessage);
//         }
//     }
// }

static void vDirectionTask(void* pvParameters) {
    while(true) {
        get_direction();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vLineTask(void* pvParameters) {
    while(true) {
        detectLine();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vFloodfillTask(void* pvParameters) {
    while(true) {
        // Initialize the original maze
        struct wall_maze maze;
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                for (int k = 0; k < 4; k++) {
                    maze.cells[i][j].walls[k] = 0; // Set all walls to false initially
                }
                maze.cells[i][j].visited = false; // Set visited to false initially
            }
        }

        struct dist_maze distances;
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                distances.distance[i][j] = UNKNOWN; // Set distances to -1 initially 
            }
        }

        //Start of the MAZE
        struct coor start;
        start.x = START_X;
        start.y = START_Y;

        // //Initialize the queue
        // initializeQueue(&flood_queue);

        // // Enqueue the first cell
        // enqueue(&flood_queue, start);

        floodfill_mapping(&maze, &distances, start, NORTH);
    }
}