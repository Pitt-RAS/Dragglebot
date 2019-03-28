#include <Servo.h>
#include "SBUS.h"

#define KPR 0.5
#define KIR 0.1
#define KDR 0.01
#define KPL 0.5
#define KIL 0.1
#define KDL 0.01

int target = 50;
float errorL = 0;
float errorR = 0;
float prevErrorL = target; //not sure why this is target
float prevErrorR = target;
float integralL = 0;
float integralR = 0;
//ResetSensors();

// a SBUS object, which is on hardware
// serial port 1
SBUS x8r(Serial1);

Servo left[2];
Servo right[2];

// channel, fail safe, and lost frames data
uint16_t channels[16];
bool failSafe;
bool lostFrame;

void pid(int target) {
 

 //loop part
 errorL = target - SensorValue[ltEncoder];
 errorR = target - SensorValue[rtEncoder];
 integralL = integralL + errorL;
 integralR = integralR + errorR;

//reset integral to 0 if error is small or large
 if(abs(errorL) < 5|| errorL > 500) {
  integralL = 0;
 }
 
 if(abs(errorR) < 5|| errorR > 500) {
  integralR = 0;
 }

//calculate derivatives and update error
 float derivativeL = errorL - prevErrorL;
 float derivativeR = errorR - prevErrorR;
 prevErrorL = errorL;
 prevErrorR = errorR;

//calculate power for motors
 double lPwr = KPL*errorL + KIL*integralL + KDL*derivativeL;
 double rPwr = KPR*errorR + KIR*integralR + KDR*derivativeR;

//correct for straightness?
if((abs(SensorValue[ltEncoder]) > abs(SensorValue[rtEncoder]+ 5)) && errorL > 50) {
    lPwr = lPwr - (lPwr/abs(lPwr))*10;
}
if((abs(SensorValue[ltEncoder]) < abs(SensorValue[rtEncoder] - 5)) && errorR > 50) {
    rPwr = rPwr - (rPwr/abs(rPwr))*10;
}
  double steering = 0;
  double left_m = throttle + steering;
  double right_m = throttle - steering;
  set_speeds(left_m, right_m);
}

void set_speeds(double left_speed, double right_speed) {
  right_speed *= -1;
  
  left_speed *= 500;
  left_speed += 1500;

  right_speed *= 500;
  right_speed += 1500;

  left[0].writeMicroseconds(left_speed);
  left[1].writeMicroseconds(left_speed);

  right[0].writeMicroseconds(right_speed);
  right[1].writeMicroseconds(right_speed);
}

unsigned long last_updated = millis();

void setup() {
  // begin the SBUS communication
  x8r.begin();
  Serial.begin(9600);

  left[0].attach(20);
  left[1].attach(21);

  right[0].attach(22);
  right[1].attach(23);

  set_speeds(0, 0);
}

void loop() {

  // look for a good SBUS packet from the receiver
  
  if(x8r.read(&channels[0], &failSafe, &lostFrame)){
    last_updated = millis();
    
    if ( channels[5] == 1811 ) {
      double throttle = channels[2] - 992;
      throttle /= 819.0;
  
      double steering = channels[0] - 992;
      steering /= 819.0;

      pid(target);
      //double left_m = throttle + steering;
      //double right_m = throttle - steering;
  
      //set_speeds(left_m, right_m);
    }
    else
      set_speeds(0, 0);
  }

  if ( failSafe )
    set_speeds(0, 0);

  if ( millis() - last_updated > 500 )
    set_speeds(0, 0);
}
