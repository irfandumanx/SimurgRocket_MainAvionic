#include "BNO055.h"

SimpleKalmanFilter rollKalmanFilter(0.5, 0.5, 0.01);
SimpleKalmanFilter yawKalmanFilter(0.5, 0.5, 0.01);
SimpleKalmanFilter pitchKalmanFilter(0.5, 0.5, 0.01);

BNO055::BNO055(int32_t sensorID, uint8_t address) {
  this->bno = new Adafruit_BNO055(sensorID, address);
    if (!bno->begin())
  {
    Serial.println("No BNO055 detected");
    while (1);
  }else Serial.println("BNO begin success");
  tone(27, 10);
  delay(200);
  noTone(27);
  delay(50);
}

BNO055Data BNO055::getSensorData() {
  sensors_event_t orientationData , linearAccelData, gravityData;
  bno->getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno->getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno->getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  float x = orientationData.orientation.x; // donme verileri
  float y = orientationData.orientation.y; // gerekirse kalmani buraya uygula
  float z = orientationData.orientation.z;

  float gX = gravityData.acceleration.x; // g kuvvetinden ivme
  float gY = gravityData.acceleration.y;
  float gZ = gravityData.acceleration.z;
  //Z 95DEN KUCUKSE
  //Y 75den buyukse;

  /*Serial.print("Kalman X: ");
  Serial.println(abs(rollKalmanFilter.updateEstimate(x)));
  Serial.print("Kalman Y: ");
  Serial.println(abs(yawKalmanFilter.updateEstimate(y)));
  Serial.print("Kalman Z: ");
  Serial.println(abs(pitchKalmanFilter.updateEstimate(z)));*/

  x = abs(rollKalmanFilter.updateEstimate(x));
  y = abs(yawKalmanFilter.updateEstimate(y));
  z = abs(pitchKalmanFilter.updateEstimate(z));

  bnoData->x = x;
  bnoData->y = y;
  bnoData->z = z;
  bnoData->gX = gX;
  bnoData->gY = gY;
  bnoData->gZ = gZ;
  return *bnoData;
}
