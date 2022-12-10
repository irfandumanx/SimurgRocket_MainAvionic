#ifndef RF_H
#define RF_H
#include "Arduino.h"
#include "BMP280.h"
#include "BNO055.h"
#include "GPS.h"

class RF {

public:
  HardwareSerial *serial;
  RF(HardwareSerial *serial);
  void sendData(BMP280Data bmpData, BNO055Data bnoData, GPSData gpsData, uint8_t rocketState);
private:
  uint8_t calculateParityByte(BMP280Data bmpData, BNO055Data bnoData, GPSData gpsData, uint8_t rocketState);
 };

#endif
