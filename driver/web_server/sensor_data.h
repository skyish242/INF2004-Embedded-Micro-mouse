#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

typedef struct{
    char map_data[1024];
} SensorData;

extern SensorData sensorData;

#endif