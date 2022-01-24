#ifndef CONTROLLER_H__
#define CONTROLLER_H__
#include <inttypes.h>
//#include <Adafruit_PWMServoDriver.h>
#include "DataAnalyse.h"
#include <Servo.h>
#include <stdlib.h>



#define PIN_SERVO1 (9)
#define PIN_SERVO2 (10)
#define PIN_TRIGGER 13
#define PIN_ECHO    12
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SENSOR_MAX_RANGE 300 // in cm

#define SERVO_BACK_DISTANCE 70
#define SERVO_FORWARD_DISTANCE 115
#define SERVO_CENTRED 95
#define STEP_DELAY 160
#define STOP_DISTANCE 10  //stop distance in cm

//#include <String.h>
//#define WRAPPERACTIVE

#ifdef DANCE
char walkingForward[] = {50, 105,
                         115, 115,
                         105, 50,
                         70, 70
                        };

#endif
#ifdef WALKRIGHT
char walkingForward[] = {45, 90,
                         45, 135,
                         55, 90,
                         90, 90
                        };
#endif
#if 0
char walkingForward[] = {90, 45,
                         135, 45,
                         90, 55,
                         90, 90
                        };
#else
char walkingForward[] = {SERVO_BACK_DISTANCE, SERVO_FORWARD_DISTANCE,
                         SERVO_FORWARD_DISTANCE, SERVO_FORWARD_DISTANCE,
                         SERVO_FORWARD_DISTANCE, SERVO_BACK_DISTANCE,
                         SERVO_BACK_DISTANCE, SERVO_BACK_DISTANCE
                        };
                        #endif
class Controller {  
  private:
  DataAnalyse *m_Da;
  int32_t m_Distance;
  uint32_t m_ValidDistance;
  uint32_t m_WalkDirection;
  Servo m_Front;
  Servo m_Back;
#ifndef WRAPPERACTIVE
  //Adafruit_PWMServoDriver m_Pwm;
  
  #endif

  
  public:
  enum {
    DISTANCE_UNCHANGED  = 0,
    DISTANCE_SHORT      = 1,
    DISTANCE_FAR        = 2,
    FRONT               =  9,
    BACK                =  10,
    LEFTFORWARD         = 110,
    CENTER              =  95,
    RIGHTFORWARD        =  70,
    ACTION_TURN_LEFT = 1,
    ACTION_TURN_RIGHT = 2,
  };
  
  typedef double float64_t;
  
  Controller() {
    
  }
  ~Controller(){
    if(m_Da)
    {
      delete(m_Da);
      m_Da = NULL;
    }
  }

  void Init() {
    Serial.println("Ab hier ist der Fehler");
    m_Da = new DataAnalyse();
#ifndef WRAPPERACTIVE
Serial.println("DEBUG1 hallo");
#if 1
    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    m_Front.attach(10,500,2500);
    m_Back.attach(9,500,2500);
    Serial.println("Attached");

#else
    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    m_Pwm = Adafruit_PWMServoDriver();
    m_Pwm.begin();
    //m_Pwm.setOscillatorFrequency(27000000);
    m_Pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
#endif
#endif
    Serial.println("Init done\n");
  }

  
  uint32_t CalculatePulse(uint32_t angle) {
    uint32_t rv = angle*11.1111+500;//(angle/180+1)*1000;//;

    //Serial.println("Pulse: "+ String(rv));
    return rv;
  }
  
  void SetMovementDirection(uint32_t state)
  {
    m_WalkDirection = state;
  }
  void Turn(uint32_t walkdirection)
  {
    Serial.println("m_WalkDirection: " + String(walkdirection));
    switch(walkdirection)
    {
      default:
        break;
      case ACTION_TURN_LEFT:
        //walkBackAndTurnLeft();
        moveLeft();
        break;
      case ACTION_TURN_RIGHT:
        moveRight();
        break;
    }
    
  }
  uint32_t getDistance()
  {
    //Serial.println("m_Da->getValidDistance(): " + String(m_Da->getValidDistance()));
    return m_Da->getDataResult();
  }
#ifdef WRAPPERACTIVE
	int32_t distance = (rand() % 100);
	//Serial.println("Random distance value: "+ String(distance));
	void 
	MeasureDistance()
	{
	  m_Distance = distance + ( rand() % 80 );
    m_Distance = ( m_Distance < 1) ? STOP_DISTANCE : m_Distance;
    //Serial.println("Random distance value: "+ String(m_Distance));
    m_Da->ProceedValue(m_Distance);
  }

#else
  void MeasureDistance(){
    uint32_t distance = 0;
    digitalWrite(PIN_TRIGGER, LOW);
    delayMicroseconds(2);
    
    digitalWrite(PIN_TRIGGER, HIGH);
    delayMicroseconds(10);
    
    digitalWrite(PIN_TRIGGER, LOW);
    uint32_t duration = pulseIn(PIN_ECHO, HIGH);
    distance = (duration)/58.82;
    
    Serial.println("Distance raw: "+ String(distance));
    if ( ( distance < SENSOR_MAX_RANGE ) && (distance > 0 ) ) {
      //Serial.println("Valid");
      m_Da->ProceedValue(distance);
    }
  }
#endif
  

  void moveLeft() {
    Serial.println("Turn Left");
    RotateServo(BACK, 110);
    RotateServo(FRONT, 140);
    delay(100);
    RotateServo(FRONT, CENTER);
    delay(50);
    RotateServo(BACK, CENTER);
    delay(140);
  }

  
  void moveRight() {
    Serial.println("Turn Right");
    RotateServo(BACK, 70);
    RotateServo(FRONT, 40);
    delay(100);
    RotateServo(FRONT, CENTER);
    delay(50);
    RotateServo(BACK, CENTER);
    delay(140);
  }

  void stepForward() 
  {
    for (int n = 0; n < 4; n++) 
    {
      RotateServo(FRONT,walkingForward[n * 2]);
      RotateServo(BACK,walkingForward[(n * 2) + 1]);
      delay(STEP_DELAY);
    }
  }
void walkBackAndTurnLeft()  {
  Serial.println("Walkbackandturn left");
  for (int n = 0; n < 14; n++) {
    RotateServo(FRONT, CENTER);
    RotateServo(BACK, SERVO_BACK_DISTANCE - 40);
    delay(200);
    RotateServo(FRONT,SERVO_FORWARD_DISTANCE);
    RotateServo(BACK,SERVO_FORWARD_DISTANCE + 20);
    delay(300);
  }
  RotateServo(FRONT,CENTER);
  RotateServo(BACK,CENTER);
  delay(300);
}

  
  void RotateServo(uint32_t id, uint32_t angle)
  {
     uint32_t pulse = CalculatePulse(angle);
#ifdef WRAPPERACTIVE
     Serial.println("Angle " + String(pulse) + " at id " + String(id) + ". ");
#else
    if( id == FRONT)
    {
       m_Front.write(pulse);
    }
    if( id == BACK)
    {
      m_Back.write(pulse);
    }
#endif
  }


};
#endif
