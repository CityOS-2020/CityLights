/*
 * Pins for connecting NRF24L01 transreciver:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 * CE -> 8
 * CSN -> 7
 */
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

void setup(){
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  Mirf.spi = &MirfHardwareSpi;   
  Mirf.init();
   
  Mirf.setRADDR((byte *)"serv1"); // setting up reciving address
  Mirf.payload = sizeof(AirQualityData);   
  Mirf.config();
}

void loop(){
  byte data[Mirf.payload];
  AirQualityData dataSet;
   
  if(!Mirf.isSending() && Mirf.dataReady()){    
     
    Mirf.getData((byte *)&dataSet);
    Serial.print("B"); // begin character
    Serial.print(dataSet.temperature);
    Serial.print("T"); // some kind of 'code character' needed for parsing numeric data from string
    Serial.print(dataSet.humidity);
    Serial.print("H"); // the same thing as previous
    Serial.print(dataSet.dust);
    Serial.print("E"); // end character
  }
}
