#include "GPS.h"

GPS::GPS(HardwareSerial *serial) {
  this->gps = new Adafruit_GPS(serial);
  gps->begin(9600);
  gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  gps->sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
  gps->sendCommand(PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ);
  gps->sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
  gps->sendCommand(PGCMD_ANTENNA);
  gps->sendCommand(PMTK_Q_RELEASE);
  Serial.println("GPS started succesfully");
  tone(27, 10);
  delay(200);
  noTone(27);
  delay(500);
}

void GPS::setSensorData() {
  gps->read();
  if (gps->newNMEAreceived()) {
    if (!gps->parse(gps->lastNMEA()))
    return;
  }
}

GPSData GPS::getSensorData() {
  if (gps->fix) {
    gpsData->altitude = gps->altitude;
    gpsData->longitude = gps->longitudeDegrees;
    gpsData->latitude = gps->latitudeDegrees;
    gpsData->satellites = gps->satellites;
  }else {
    gpsData->altitude = 0;
    gpsData->longitude = 0;
    gpsData->latitude = 0;
    gpsData->satellites = gps->satellites;
  }
  return *gpsData;
}

/*std::pair<double, double> GPS::predictGpsData(float azimuthAngle, float speed, float prevLat, float prevLon) {
  const float x = speed * sin(azimuthAngle * PI / 180) * GPS_TRANSITION;
  const float y = speed * cos(azimuthAngle * PI / 180) * GPS_TRANSITION;

  double lat = prevLat + (180 / PI) * (y / WORLD_RADIUS);
  double lon = prevLon + (180 / PI) / sin(prevLat * PI / 180) * (x / WORLD_RADIUS);

  return std::make_pair(lat, lon);
  
}*/
