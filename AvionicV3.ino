#include "BMP280.h"
#include "GPS.h"
#include "BNO055.h"
#include "RF.h"
#include <SD.h>
#include <SPI.h>
//bno bmp gps sdcard
BMP280* bmp;
GPS* gps;
BNO055* bno;
RF* rf;
File avionicFile;
const char* fileName = "AvionicData.txt";
unsigned char phase1 = 25;
unsigned char phase2 = 24;
uint32_t timer = 0;
unsigned char buzzerPin = 27;
unsigned char ledPin = 26;
unsigned char teensyLedPin = 13;
uint8_t rocketState = 1;
bool faz1 = false;
bool faz2 = false;
float maxAltitude = 0.0f;
uint32_t ejectSeatTimer = 0;
bool ejectSeatVerify = false;
float systemWorkTime = 0;
bool turn = false;
void setup() {
  Serial.begin(115200);
  pinMode(teensyLedPin, OUTPUT);
  pinMode(phase1, OUTPUT);
  pinMode(phase2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(teensyLedPin, HIGH);
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, LOW);
  bno = new BNO055(55, 0x28);
  bmp = new BMP280(&Wire, BMP::eSdoLow);
  rf = new RF(&Serial5);
  gps = new GPS(&Serial1);
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("SD initialization failed!");
    return;
  }
  Serial.println("SD initialization done.");
  tone(27, 10);
  delay(200);
  noTone(27);
}

void loop() {
  gps->setSensorData();
  if (millis() - timer > 50) {
    //=========================================
    BNO055Data bnoData = bno->getSensorData();
    //=========================================
    BMP280Data bmpData = bmp->getSensorData();
    //=========================================
    GPSData gpsData = gps->getSensorData();
    //=========================================
    if(!faz2) {
      if(bmpData.estimated_alti>maxAltitude) {
        maxAltitude = bmpData.estimated_alti;
      }

      if(bnoData.z <= 95 || bnoData.y >= 75) turn = true;
      
      //Ek Patlatma Algoritmasi
      if(!ejectSeatVerify && bmpData.estimated_alti > 3000) {
        ejectSeatVerify = true;
        ejectSeatTimer = millis();
      }

      if(ejectSeatVerify && millis() - ejectSeatTimer > 7000) {
        rocketState = 2;
        digitalWrite(phase1, HIGH);
        faz1 = true;
      }

      //Ana Patlatma Algoritmasi
      if(bmpData.estimated_alti > 2200 && maxAltitude-bmpData.estimated_alti > 25 && turn) {
        rocketState = 2;
        digitalWrite(phase1, HIGH);
        faz1 = true;
      }
      if(faz1 && bmpData.estimated_alti < 600) {
        rocketState = 3;
        digitalWrite(phase2, HIGH);
        faz2 = true;
      }
    }else {
      rocketState = 4;
      if(bmpData.estimated_alti < 200) {
        tone(buzzerPin, 1000);
      }
    }
    //=========================================
    rf->sendData(bmpData, bnoData, gpsData, rocketState);
    //=========================================
    avionicFile = SD.open(fileName, FILE_WRITE);
    if (avionicFile) {
      avionicFile.print(millis() / 1000.0f);
      avionicFile.print("*");
      avionicFile.print(bmpData.estimated_alti);
      //================================================================
      avionicFile.print("*");
      avionicFile.print(bnoData.x);
      avionicFile.print("*");
      avionicFile.print(bnoData.y);
      avionicFile.print("*");
      avionicFile.print(bnoData.z);
      avionicFile.print("*");
      avionicFile.print(bnoData.gX);
      avionicFile.print("*");
      avionicFile.print(bnoData.gY);
      avionicFile.print("*");
      avionicFile.print(bnoData.gZ);
      //================================================================
      avionicFile.print("*");
      avionicFile.print(gpsData.latitude, 6);
      avionicFile.print("*");
      avionicFile.print(gpsData.longitude, 6);
      avionicFile.print("*");
      avionicFile.print(gpsData.altitude);
      //================================================================
      avionicFile.print("*");
      avionicFile.print(rocketState);
      //================================================================
      avionicFile.print("*");
      avionicFile.print(bnoData.y);
      avionicFile.println();
      avionicFile.close();
    }
    timer = millis();
  }
}
