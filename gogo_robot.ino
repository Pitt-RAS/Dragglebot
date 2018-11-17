#include <Servo.h>
#include "SBUS.h"

// a SBUS object, which is on hardware
// serial port 1
SBUS x8r(Serial1);

Servo left[2];
Servo right[2];

// channel, fail safe, and lost frames data
uint16_t channels[16];
bool failSafe;
bool lostFrame;
int count = 0;

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
  
  /*if(x8r.read(&channels[0], &failSafe, &lostFrame)){
    last_updated = millis();
    
    if ( channels[5] == 1811 && count!=1 ) {
      double throttle = channels[2] - 992;
      throttle /= 819.0;
  
      double steering = 1100 - 992;
      steering /= 819.0;
  
      double left_m = throttle + steering;
      double right_m = throttle - steering;
      Serial.println(channels[0]);
      /*set_speeds(0.25, 0.25);
      delay(5000);
      for(int i = 0; i<10; i++){
        set_speeds(left_m, right_m);
        delay(100);
      }
      for(int i1 = 0; i1<10; i1++){
        set_speeds(right_m, left_m);
        delay(100);
      }
      set_speeds(0, 0);
      delay(1000);
      count++;
    }
    else
      set_speeds(0, 0);
       
  }

  if ( failSafe )
    set_speeds(0, 0);

  if ( millis() - last_updated > 500 ){
    set_speeds(0, 0); 
  }*/

      double throttle = channels[2] - 992;
      throttle /= 819.0; 
      double steering = 1100 - 992;
      steering /= 819.0;
      double left_m = 0 + steering;
      double right_m = 0 - steering; 
         
      if(Serial.read() == 'Y'){
        set_speeds(0.2, 0.2);
      }
      else if(Serial.read() == 'N' || Serial.read() == 'R' || Serial.read() == 'L'){
        set_speeds(0, 0);
      }   
      
      if(Serial.read() == 'R'){
        
        set_speeds(0.3, -0.1);       
        /*delay(25);*/
      }
      else if(Serial.read() == 'L'){
        set_speeds(-0.1, 0.3);
        
      }
      
    
}
