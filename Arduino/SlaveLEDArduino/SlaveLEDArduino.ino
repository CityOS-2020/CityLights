#include "TimerOne.h"

#define lowLevel 512 // for easier adjustment of light intensity
#define midLevel 600
#define highLevel 800

//
// Global variables declaration - BEGIN
//
int pirPin = 4; // PIR motion sensor
int pwmPin = 9; // Dimming LED lights
int photoPin = 1; // Analog input for photosensitive sensor

int duty = 0;
int oldDuty = 0;
//
// Global variables declaration - END
//

//
// PIR motion sensor - BEGIN
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
// Photosensitive sensor - BEGIN
//
int getPhotoValue()
{
  return analogRead(photoPin);
}

//
// LED lights dimming - BEGIN
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
      else { 
        oldDuty=oldDuty+1; 
        Timer1.setPwmDuty(pwmPin, oldDuty); 
      }
    }
    if(duty < oldDuty) {
      //oldDuty--;
      if(oldDuty < 2) Timer1.setPwmDuty(pwmPin, 0);
      else { 
        oldDuty = oldDuty-1; 
        Timer1.setPwmDuty(pwmPin, oldDuty); 
      }
    }
  }
}

//
// LED lights dimming - END
//

void setup() {
  setupPIR();
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
        // set intensity to some experimentaly rated value
        setBrightness(300);
      }
      else if(photoValue < highLevel)
      {
        // set intensity to some experimentaly rated value
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
