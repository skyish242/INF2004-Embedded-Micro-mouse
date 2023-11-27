
#include <stdio.h>
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"

#include "sensor_data.h"
#include "motor.h"
#include "floodfill.h"

char* map_stringfy(int map[16][16]){
    // Create a buffer to hold the JSON string
    char jsonBuffer[1024];
    char* ptr = jsonBuffer;  // Create a pointer to track the position in the buffer

    // Start building the JSON string
    ptr += snprintf(ptr, sizeof(jsonBuffer), "[");

    // Loop through the array to build the JSON string
    for (int i = 0; i < 11; i++) {
        ptr += snprintf(ptr, sizeof(jsonBuffer) - (ptr - jsonBuffer), "[");
        for (int j = 0; j < 9; j++) {
            ptr += snprintf(ptr, sizeof(jsonBuffer) - (ptr - jsonBuffer), "%d", map[i][j]);
            if (j < 8) {
                ptr += snprintf(ptr, sizeof(jsonBuffer) - (ptr - jsonBuffer), ",");
            }
        }
        ptr += snprintf(ptr, sizeof(jsonBuffer) - (ptr - jsonBuffer), "]");
        if (i < 10) {
            ptr += snprintf(ptr, sizeof(jsonBuffer) - (ptr - jsonBuffer), ",");
        }
    }

    // End the building of JSON String
    ptr += snprintf(ptr, sizeof(jsonBuffer) - (ptr - jsonBuffer), "]");

    // Null-terminate the string
    *ptr = '\0';

    return ptr;
}

// CGI handler which is run when a request for /led.cgi is detected
const char * cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Check if an request for LED has been made (/led.cgi?led=x)
    if (strcmp(pcParam[0] , "led") == 0){
        // Look at the argument to check if LED is to be turned on (x=1) or off (x=0)
        if(strcmp(pcValue[0], "0") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        else if(strcmp(pcValue[0], "1") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    }
    
    // Send the index page back to the user
    return "/index.shtml";
}

// CGI handler which is run when a request for /get_map.cgi is detected
const char * cgi_map_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    //Convert the map array from sensor fusion to string
    int map[16][16] = {
        {1, 2, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 2, 1}
    };
    //Convert to string
    char * map_string = map_stringfy(map);

    // Copy the map_string to sensorData.map_data
    snprintf(sensorData.map_data, sizeof(sensorData.map_data), "%s", map_string);

    printf("Map: \n%s", sensorData.map_data);

    //Call ssi to send the map
    ssi_init(true);

    return "/index.shtml";
}

// CGI handler which is run when a request for /car_move.cgi is detected
const char * cgi_car_movement_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    // Init motor
    //motorInit();
    // Check if the car is ordered to move (/car_movement.cgi?move=x)
    if (strcmp(pcParam[0] , "move") == 0){
        if (strcmp(pcValue[0] , "left") == 0){
            moveLeft();
            printf("Left initiated\n");
        }else if(strcmp(pcValue[0] , "right") == 0){
            moveRight();
            printf("Right initiated");
        }else if(strcmp(pcValue[0] , "forward") == 0){

            moveForward();
            
            printf("Forward initiated");
        }else if(strcmp(pcValue[0] , "backward") == 0){
            moveBackward();
            printf("Backward initiated");
        }
    } 
    
    return "/index.shtml";
}

const char * cgi_start_map_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    mapping_start();
    return "/index.shtml";
}

// tCGI Struct
// Fill this with all of the CGI requests and their respective handlers
static const tCGI cgi_handlers[] = {
    {
        // Html request for "/led.cgi" triggers cgi_handler
        "/led.cgi", cgi_led_handler
    },
    {
        // Html request for "/led.cgi" triggers cgi_handler
        "/get_map.cgi", cgi_map_handler
    },
    {
        // Html request for robot car movement
        "/car_movement.cgi", cgi_car_movement_handler
    },
    {
        // Html request for robot car movementu
        "/start_map.cgi", cgi_start_map_handler
    },
};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 4);
}