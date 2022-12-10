#include "BMP280.h"

SimpleKalmanFilter temperatureKalmanFilter(1.5, 1.5, 0.01);
SimpleKalmanFilter pressureKalmanFilter(1.5, 1.5, 0.01);
SimpleKalmanFilter altitudeKalmanFilter(1.5, 1.5, 0.01);

BMP280::BMP280(TwoWire *pWire, BMP::eSdo_t eSdo) {
  this->bmp = new BMP(pWire, eSdo);
  bmp->reset();

  Serial.println("BMP try read data");
  while(bmp->begin() != BMP::eStatusOK) {
    Serial.println("BMP begin faild");
    printLastOperateStatus(bmp->lastOperateStatus);
    delay(2000);
  }
  Serial.println("BMP begin success");
  tone(27, 10);
  delay(200);
  noTone(27);
  delay(500);
  firstAltitude = bmp->calAltitude(SEA_LEVEL_PRESSURE, bmp->getPressure());
}

BMP280Data BMP280::getSensorData() {
  bmpData->temperature = bmp->getTemperature();
  bmpData->pressure = bmp->getPressure();
  bmpData->altitude = bmp->calAltitude(SEA_LEVEL_PRESSURE, bmpData->pressure) - firstAltitude;
  bmpData->estimated_temp = temperatureKalmanFilter.updateEstimate(bmpData->temperature);
  bmpData->estimated_press = pressureKalmanFilter.updateEstimate(bmpData->pressure);
  bmpData->estimated_alti = altitudeKalmanFilter.updateEstimate(bmpData->altitude);
  return *bmpData;
}

void BMP280::printLastOperateStatus(BMP::eStatus_t eStatus)
{
  switch(eStatus) {
    case BMP::eStatusOK:    Serial.println("everything ok"); break;
    case BMP::eStatusErr:   Serial.println("unknow error"); break;
    case BMP::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
    case BMP::eStatusErrParameter:    Serial.println("parameter error"); break;
    default: Serial.println("unknow status"); break;
  }
}
