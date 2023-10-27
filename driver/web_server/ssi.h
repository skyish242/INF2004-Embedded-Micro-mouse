#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

#include "sensor_data.h"

// SSI tags - tag length limited to 8 bytes by default
const char * ssi_tags[] = {"led", "map1", "map2", "map3", "map4"};
const char* map_data = sensorData.map_data;
char* map_piece[4] = { "", "", "", ""};

u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
  size_t printed;
  switch (iIndex) {
  case 0: // led
    {
      bool led_status = cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
      if(led_status == true){
        printed = snprintf(pcInsert, iInsertLen, "ON");
      }
      else{
        printed = snprintf(pcInsert, iInsertLen, "OFF");
      }
    }
    break;
  case 1: // map1
    {
      const char * map = map_piece[0];
      printed = snprintf(pcInsert, iInsertLen, map);
    }
    break;
  case 2: // map2
    {
      if (map_piece[1] != NULL) {
        const char * map = map_piece[1];
        printed = snprintf(pcInsert, iInsertLen, map);
      }
    }
    break;
  case 3: // map3
    {
      if (map_piece[2] != NULL) {
        const char * map = map_piece[2];
        printed = snprintf(pcInsert, iInsertLen, map);
      }
    }
    break;
  case 4: // map4
    {
      if (map_piece[3] != NULL) {
        const char * map = map_piece[3];
        printed = snprintf(pcInsert, iInsertLen, map);
      }
    }
    break;
  default:
    printed = 0;
    break;
  }

  return (u16_t)printed;
}

void map_splitting(){
  size_t map_data_len = strlen(map_data);
  size_t chunk_size = 190; //iInsertLen size
  int length = sizeof(map_piece) / sizeof(map_piece[0]); // length of map_piece array

  for (int i = 0; i < length; i++) {
      size_t start_index = i * chunk_size;
      size_t end_index = start_index + chunk_size;

      if (start_index < map_data_len) {
          if (end_index > map_data_len) {
              end_index = map_data_len;
          }

          size_t chunk_length = end_index - start_index;

          map_piece[i] = (char*)malloc(chunk_length + 1); // +1 for null-termination

          strncpy(map_piece[i], map_data + start_index, chunk_length);
          map_piece[i][chunk_length] = '\0'; // Null-terminate the chunk
      } else {
          map_piece[i] = NULL; // Set to NULL if there are no more chunks
      }
  }
}

// Initialise the SSI handler
void ssi_init(bool map) {
  
  if(map) {
    map_splitting();
  }	

  http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}
