#include <Wire.h>
#include "StateController.h"

StateController *sc;


void setup() 
{
	Serial.begin(9600);
 sc = new StateController();
}


void loop(){
	sc->StateMachine();
 delay(100 );
  
}
