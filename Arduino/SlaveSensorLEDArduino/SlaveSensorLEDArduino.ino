// NRF24L01 configuration
/*
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
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
#include "TimerOne.h"

#define lowLevel 512
#define midLevel 600
#define highLevel 800

//
// Global variables declaration - START
//
int ledPin = 2; // Dust sensor
int analogOutPin = 0; // Dust sensor
int DHpin = 6; // Temperature and humidity sensor
int pirPin = 4; // PIR motion sensor
int pwmPin = 9; // Dimming LED lights
int photoPin = 1; // Analog input for photosensitive sensor
int analogBatteryPin = 2; // Analog input for battery status indication
int switchToAC = 3; // Digital output for switching power supply

int duty = 0;
int oldDuty = 0;

int preload; // TIMER2, 10s period of data acquisition
int interruptCounter; // TIMER2, 10s period of data acquisition
//
// Global variables declaration - END
//

//
// Setting up interrupt on Timer2 - START
//
void setupInterruptTimer2()
{
  // timer initialization
  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
  preload = 0;
  TCNT2 = preload; // Setting up preload value
  TCCR2B = (1<<CS22) | (1<<CS20)| (1<<CS21); //Setting up prescaler
  TIMSK2 |= (1 << TOIE2);
  interruptCounter = 0;
  interrupts();
}

// Interrupt routine
ISR(TIMER2_OVF_vect)
{
  TCNT2 = preload; // set preload value again
  interruptCounter++;
  // value 610 calculated for sampling period 10s
  switchPowerSupply();
  if(interruptCounter == 610)
  {
    noInterrupts(); // keeps acquisition and transmition uninterrupted
    Serial.println("Taf!");
    collectAndTransmitData();
    interruptCounter = 0;
    interrupts(); // turn interrupts back on
  }
}
//
// Setting up interrupt on Timer2
//

struct AirQualityData 
{
  byte temperature;
  byte humidity;
  float dust;
};

void setupNRF24L01(){
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  pinMode(A0, INPUT);
  
  Mirf.setRADDR((byte *)"clie1"); // configure reciving address   
  Mirf.payload = sizeof(AirQualityData); // payload on client and server must bi the same
  /*
   * To change channel:
   * Mirf.channel = 10;
   * NB: Make sure channel is legal in your area.
   */
  Mirf.config();
  
  Serial.println("Beginning ... "); 
}

//
// Dust sensor - START
// 
// PIN1 -> over the 150E resistor to Vcc
// PIN2 -> GND
// PIN3 -> LED pin
// PIN4 -> GND
// PIN5 -> AnalogOut
// PIN6 -> Vcc
//
void setupDust() 
{
  pinMode(ledPin, OUTPUT);
}

float getDataFromDustSensor() {
  // ledPin in void setup() must be declared as output
  
  int sensorValue;
  float dustDensity;
  
  digitalWrite(ledPin, HIGH); // turn OFF led
  delayMicroseconds(1000); // led is OFF for 1ms - to make sure that output is 0
  digitalWrite(ledPin, LOW); 
  delayMicroseconds(280); // turn ON led for 0.28ms
  sensorValue = analogRead(analogOutPin);
  delayMicroseconds(40);
  digitalWrite(ledPin, HIGH); 
  delayMicroseconds(8680); // turn OFF led for another 8.68ms
  
  // code for voltage-to-concentration conversion (according to datasheet)
  // linear equation taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 0.17 * (sensorValue * 5.0 / 1023.0) - 0.1;
  if(dustDensity < 0) dustDensity = 0;
  
  return dustDensity;
}
//
// Dust sensor - END
//

//
// Temperature and humidity sensor - START
//
void setupDHT () {
  pinMode (DHpin, OUTPUT);
}

byte readSingleDataFromDHT () {
  byte data;
  for(int i = 0; i < 8; i ++) {
    if(digitalRead(DHpin) == LOW) {
      while(digitalRead(DHpin) == LOW); // wait for 50us
      delayMicroseconds(30); // determine the duration of the high level to determine the data is '0 'or '1'
      if(digitalRead(DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while(digitalRead(DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}
 
void getDataFromDHTSensor(byte *temperature, byte *humidity) {
  // passing by reference
  byte dat [5];
  digitalWrite(DHpin, LOW); // bus down, send start signal
  delay(30); // delay greater than 18ms, so DHT11 start signal can be detected
 
  digitalWrite(DHpin, HIGH);
  delayMicroseconds(40); // Wait for DHT11 response
 
  pinMode(DHpin, INPUT);
  while(digitalRead(DHpin) == HIGH);
  delayMicroseconds(80); // DHT11 response, pulled the bus 80us
  if(digitalRead(DHpin) == LOW);
  delayMicroseconds(80); // DHT11 80us after the bus pulled to start sending data
 
  for(int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = readSingleDataFromDHT();
 
  pinMode(DHpin, OUTPUT);
  digitalWrite(DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
  
  *humidity = dat[0];
  *temperature = dat[2];
}
//
// Temperature and humidity sensor - END
//

//
// PIR motion sensor - START
//
void setupPIR() {
  pinMode(pirPin, INPUT);
}

boolean isMotionDetected()
{
  int pirValue, i;
  // to increase reliability of sensor increase number of iteration in for loop
  for (i = 0; i < 10; i++)
  {
    pirValue = digitalRead(pirPin);
    delay(200);
    if (pirValue == 0) return true;
  }
  return false;
}
//
// PIR motion sensor - END
//

//
// Photosensitive sensor - START
//
int getPhotoValue()
{
  return analogRead(photoPin);
}

//
// LED lights dimming - START
//
void setBrightness(int brightness) {
    duty = brightness;
	//duty = int(1023 * (float)((float)brightness/100.0));
        //Serial.println(duty);
}


void setupPWM(int period) {
	Timer1.initialize(period); 
	Timer1.pwm(pwmPin, 0);
	Timer1.attachInterrupt(callback);
}

void callback() {
        if(duty == oldDuty) return;
	if(duty != oldDuty) {
		if(duty > oldDuty) {
			//oldDuty++;
                        if(oldDuty > 1000) Timer1.setPwmDuty(pwmPin, 1000);
                        else { oldDuty=oldDuty+1; Timer1.setPwmDuty(pwmPin, oldDuty); }
		}
		if(duty < oldDuty) {
			//oldDuty--;
                        if(oldDuty < 2) Timer1.setPwmDuty(pwmPin, 0);
                        else { oldDuty = oldDuty-1; Timer1.setPwmDuty(pwmPin, oldDuty); }
		}
	}
}
//
// LED lights dimming - END
//

//
// Data acquisition and transmition - START
//
void setupPowerSwitch()
{
  pinMode(switchToAC, OUTPUT);
  digitalWrite(switchToAC, LOW);
}

void switchPowerSupply()
{
  int batteryVoltage = analogRead(A2);
  // Reading battery voltage via voltage divider with 1:2 ratio
  if(batteryVoltage < 800) digitalWrite(switchToAC, HIGH);
  else if(batteryVoltage > 830) digitalWrite(switchToAC, LOW);
  // batteryVoltage = 800 <=> ~12V, if less switch to network
}

void collectAndTransmitData()
{
  // PICK DATA
  byte temperature, humidity;
  getDataFromDHTSensor (&temperature, &humidity);
  float dustData = getDataFromDustSensor();
  
  AirQualityData data;
  data.temperature = temperature;
  data.humidity = humidity;
  data.dust = dustData;
  
  // Send data via NRF
  Mirf.setTADDR((byte *)"serv1");
  Mirf.send((byte *)&data);
  
  while(Mirf.isSending()){
  }
  // comment the line below for final version (to improve performance
  Serial.println("Finished sending");
}

//
// Data acquisition and transmition - END
//

void setup() {
  setupInterruptTimer2();
  setupNRF24L01();
  setupDHT();
  setupDust();
  setupPIR();
  setupPowerSwitch();
  setupPWM(5000);
  setBrightness(0);
  Serial.begin(9600);
}

 
void loop () {  
  // LED automation
  int setDelay;
  int photoValue = getPhotoValue();
  boolean motion;
  if(photoValue >= lowLevel)
  {
    // turn lights on
    motion = isMotionDetected();
    if(motion)
    {
      setDelay = 5000;
      if(photoValue < midLevel)
      {
        // set intensity to some experimentally rated value
        setBrightness(300);
      }
      else if(photoValue < highLevel)
      {
        // set intensity to some experimentally rated value
        setBrightness(700);
      }
      else
      {
        // set intensity to 100%
        setBrightness(1023);
      }
    }
    else
    {
      setDelay = 50;
      setBrightness(100);
    }
  }
  else
  {
    setDelay = 5000;
    setBrightness(0);
  }
  delay(setDelay); 
}
