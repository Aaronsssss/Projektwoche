

#include <Wire.h>

#include "Controller.h"

#define FRONT (14)
#define BACK (15)
#define LEFTFORWARD (110)
#define CENTER (90)
#define RIGHTFORWARD (70)


unsigned long duration;
unsigned int distance;
Controller *controller;


void setup() {
	
    controller = new Controller();
    controller->RotateServo(FRONT,CENTER);
  controller->RotateServo(BACK,CENTER);
  //delay(4000);
  //controller->RotateServo(FRONT,LEFTFORWARD);
  //controller->RotateServo(BACK,RIGHTFORWARD);
  
}


void loop(){
	
  //controller->MeasureDistance();
  Serial.println("xxxDistance to object: " + controller->getDistance() + " cm\n");
  delay(10);
  
  controller->stepForward();
 
  #if 0
  
  //controller->moveForward();

for(uint32_t i = 0; i < 20; i++)
{
  controller->moveLeft();
  delay(200);
}
for(uint32_t i = 0; i < 20; i++)
{
  controller->moveRight();
  delay(200);
}
delay(1000);
  
  
  
  #endif
}
