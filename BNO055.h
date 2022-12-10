#ifndef BNO055_H
#define BNO055_H
#include "Arduino.h"
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <SimpleKalmanFilter.h>

typedef struct BNO055Data {
  float x;
  float y;
  float z;
  float gX;
  float gY;
  float gZ;
} BNO055Data;

class BNO055 {
  BNO055Data *bnoData = (BNO055Data*) malloc(sizeof(BNO055Data));
public:
  BNO055(int32_t sensorID, uint8_t address);
  Adafruit_BNO055 *bno;
  BNO055Data getSensorData();
};

#endif
