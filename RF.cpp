#include "RF.h"

RF::RF(HardwareSerial *serial) {
  this->serial = serial;
  this->serial->begin(57600);
}

float angle = 0.0f;

void RF::sendData(BMP280Data bmpData, BNO055Data bnoData, GPSData gpsData, uint8_t rocketState) {
  serial->print("!");
  /*serial->print(bmpData.estimated_temp);
  serial->print("*");
  serial->print(bmpData.estimated_press);
  serial->print("*");*/
  serial->print(bmpData.estimated_alti);
  //================================================================
  serial->print("*");
  serial->print(bnoData.x);
  serial->print("*");
  serial->print(bnoData.y);
  serial->print("*");
  serial->print(bnoData.z);
  serial->print("*");
  serial->print(bnoData.gX);
  serial->print("*");
  serial->print(bnoData.gY);
  serial->print("*");
  serial->print(bnoData.gZ);
  //================================================================
  serial->print("*");
  serial->print(gpsData.latitude, 8);
  serial->print("*");
  serial->print(gpsData.longitude, 8);
  serial->print("*");
  serial->print(gpsData.altitude);
  //================================================================
  serial->print("*");
  serial->print(rocketState);
  //================================================================
  serial->print("*");
  serial->print(angle);
  //================================================================
  serial->print("*");
  serial->print(calculateParityByte(bmpData, bnoData, gpsData, rocketState)); //parity
  serial->print("|");
}

uint8_t RF::calculateParityByte(BMP280Data bmpData, BNO055Data bnoData, GPSData gpsData, uint8_t rocketState) {
  float sum = 0.0f;

  sum += bmpData.estimated_alti;
  sum += bnoData.x;
  sum += bnoData.y;
  sum += bnoData.z;
  sum += bnoData.gX;
  sum += bnoData.gY;
  sum += bnoData.gZ;
  sum += gpsData.latitude;
  sum += gpsData.longitude;
  sum += gpsData.altitude;
  sum += rocketState;
  sum += angle;

  
  return (uint8_t) sum % 256;
}
