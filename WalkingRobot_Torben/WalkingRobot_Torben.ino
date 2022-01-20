#include <Wire.h>
#include "StateController.h"

#define FRONT (14)
#define BACK (15)
#define LEFTFORWARD (110)
#define CENTER (90)
#define RIGHTFORWARD (70)
StateController *sc;
//Controller controller;


void setup() {
	Serial.begin(9600);
 Serial.println("xxxDistance to object: ");
 //sc = new StateController();
  //controller = new Controller();
  //controller.RotateServo(FRONT,CENTER);
 // controller.RotateServo(BACK,CENTER);
  //delay(4000);
}


void loop(){
	
  //controller.MeasureDistance();
 // Serial.println("xxxDistance to object: " + StateController.getDistance() + " cm\n");
  //delay(10);
  
  //controller.stepForward();
  
  
  
  
}
