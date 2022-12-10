#ifndef BMP280_H
#define BMP280_H
#define SEA_LEVEL_PRESSURE    1015.0f

#include "Arduino.h"
#include <Wire.h>
#include "DFRobot_BMP280.h"
#include <SimpleKalmanFilter.h>

typedef DFRobot_BMP280_IIC    BMP; 

typedef struct BMP280Data {
  float temperature;
  uint32_t pressure;
  float altitude;
  float estimated_temp;
  uint32_t estimated_press;
  float estimated_alti;
} BMP280Data;


class BMP280 {
  
  BMP280Data *bmpData = (BMP280Data*) malloc(sizeof(BMP280Data));
  
public:
  BMP280(TwoWire *pWire, BMP::eSdo_t eSdo);
  BMP *bmp;
  BMP280Data getSensorData();
private:
  float firstAltitude;
  void printLastOperateStatus(BMP::eStatus_t eStatus);
};


#endif
