//nano walker with ultrasonics
#define BACK_LEGS_PIN 10  //back legs on Pin10
#define BACK_LEGS_GND 8   //back legs ground connection Pin8
#define BACK_LEGS_VCC 9   //back legs power connection Pin9

#define FRONT_LEGS_PIN 3  //front legs on Pin3
#define FRONT_LEGS_VCC 2  //front legs power connection on Pin2 (front legs GND connection on GND already)

#define US_GND 19         //Ultrasonic Ground Pin D19=A5
#define ECHO_PIN 18       //Ultrasonic Echo Pin D18 = A4
#define TRIGGER_PIN 17    //Ultrasonic Trigger Pin D17 = A3
#define US_VCC 16         //Ultrasonic VCC Pin D16 = A2

#define MAXIMUM_DISTANCE 200
#define SERVO_BACK_DISTANCE 60
#define SERVO_FORWARD_DISTANCE 100
#define SERVO_CENTRED 80
#define STEP_DELAY 160
#define STOP_DISTANCE 10  //stop distance in cm

#include <Servo.h>
#include <NewPing.h>

//format: front leg position, back leg position
char walkingForward[] = {SERVO_BACK_DISTANCE, SERVO_FORWARD_DISTANCE,
                         SERVO_FORWARD_DISTANCE, SERVO_FORWARD_DISTANCE,
                         SERVO_FORWARD_DISTANCE, SERVO_BACK_DISTANCE,
                         SERVO_BACK_DISTANCE, SERVO_BACK_DISTANCE
                        };
NewPing sonarEyes(TRIGGER_PIN, ECHO_PIN, MAXIMUM_DISTANCE);
Servo servoBackLegs, servoFrontLegs;


void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  //setup Pins to connect Servos To
  pinMode(BACK_LEGS_GND, OUTPUT);  //tie pin 8 to GND
  digitalWrite(BACK_LEGS_GND, LOW);
  pinMode(BACK_LEGS_VCC, OUTPUT);  //tie pin 9 to VCC
  digitalWrite(BACK_LEGS_VCC,HIGH);
  servoBackLegs.attach(BACK_LEGS_PIN);
  
  pinMode(FRONT_LEGS_VCC, OUTPUT);  //tie pin 2 to VCC
  digitalWrite(FRONT_LEGS_VCC, HIGH);
  servoFrontLegs.attach(FRONT_LEGS_PIN);

  servoBackLegs.write(SERVO_CENTRED);
  servoFrontLegs.write(SERVO_CENTRED);

  //setup Pins for Ultrasonic Sensor
  pinMode(US_GND, OUTPUT);
  digitalWrite(US_GND, LOW);
  pinMode(US_VCC, OUTPUT);
  digitalWrite(US_VCC,HIGH);
  
  delay(2000);
  
  digitalWrite(LED_BUILTIN, LOW); //turn off built in LED
}

void loop() {
  // put your main code here, to run repeatedly:
  while (checkForObstruction()) {  //returns true if obstruction, otherwise skip this loop
    digitalWrite(LED_BUILTIN, HIGH);  //turn on LED for visual indication of obstruction
    //step back and turn
    delay(2000);
    walkBackAndTurnLeft();
    digitalWrite(LED_BUILTIN, LOW);   //turn back off again
  }
  stepForward();
}

void stepForward() {
  for (int n = 0; n < 4; n++) {
    servoFrontLegs.write(walkingForward[n * 2]);
    servoBackLegs.write(walkingForward[(n * 2) + 1]);
    delay(STEP_DELAY);
  }
}

void walkBackAndTurnLeft()  {
  for (int n = 0; n < 14; n++) {
    servoFrontLegs.write(SERVO_CENTRED);
    servoBackLegs.write(SERVO_BACK_DISTANCE - 40);
    delay(200);
    servoFrontLegs.write(SERVO_FORWARD_DISTANCE);
    servoBackLegs.write(SERVO_FORWARD_DISTANCE + 20);
    delay(300);
  }
  servoFrontLegs.write(SERVO_CENTRED);
  servoBackLegs.write(SERVO_CENTRED);
  delay(300);
}

bool checkForObstruction() {
  int distance = sonarEyes.ping_cm();
  if (distance <= STOP_DISTANCE && distance != 0) {
    return true;
  } else {
    return false;
  }
}

