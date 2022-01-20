#ifndef CONTROLLER_H__
#define CONTROLLER_H__
#include <inttypes.h>
#include <Adafruit_PWMServoDriver.h>
#define PIN_TRIGGER 11
#define PIN_ECHO    12
#define SERVOMIN  800 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  2200 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SENSOR_MAX_RANGE 300 // in cm

#define MAXIMUM_DISTANCE 200
#define SERVO_BACK_DISTANCE 60
#define SERVO_FORWARD_DISTANCE 100
#define SERVO_CENTRED 80
#define STEP_DELAY 160
#define STOP_DISTANCE 10  //stop distance in cm

//#include <String.h>

char walkingForward[] = {SERVO_BACK_DISTANCE, SERVO_FORWARD_DISTANCE,
                         SERVO_FORWARD_DISTANCE, SERVO_FORWARD_DISTANCE,
                         SERVO_FORWARD_DISTANCE, SERVO_BACK_DISTANCE,
                         SERVO_BACK_DISTANCE, SERVO_BACK_DISTANCE
                        };
class Controller {
  private:
    uint32_t m_Distance;
    Adafruit_PWMServoDriver m_Pwm;


  public:
    enum {
      FRONT        =  14,
      BACK         =  15,
      LEFTFORWARD  = 110,
      CENTER       =  90,
      RIGHTFORWARD =  70,
    };

    typedef double float64_t;

    Controller() {

      Init();
    }
    ~Controller() {

    }

    void Init() {

      //Serial.begin(9600);
      pinMode(PIN_TRIGGER, OUTPUT);
      pinMode(PIN_ECHO, INPUT);
      m_Pwm = Adafruit_PWMServoDriver();
      m_Pwm.begin();
      //m_Pwm.setOscillatorFrequency(27000000);
      m_Pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
      Serial.println("Init done\n");
    }

    String getDistance() {
      return String(m_Distance);
    }

    uint32_t CalculatePulse(uint32_t angle) {
      uint32_t rv = angle * 11.1 + 500;

      //Serial.println("Pulse: "+ String(rv));
      return rv;
    }



    uint32_t MeasureDistance() {
      uint32_t distance = 0;
      digitalWrite(PIN_TRIGGER, LOW);
      delayMicroseconds(2);

      digitalWrite(PIN_TRIGGER, HIGH);
      delayMicroseconds(10);

      uint32_t duration = pulseIn(PIN_ECHO, HIGH);
      //Serial.println("duration: "+ String(duration));
      distance = (duration / 58.0);

      if ( ( distance < SENSOR_MAX_RANGE ) || (distance > 0 ) ) {
        m_Distance = distance;
      }
      return m_Distance;
    };

    void moveForward() {
      uint32_t walkSpeed = 50;
#if 1
      RotateServo(FRONT, CENTER);
      RotateServo(BACK, CENTER);
      delay(400);
      RotateServo(BACK, LEFTFORWARD);
      delay(100);
      RotateServo(FRONT, LEFTFORWARD);
      delay(walkSpeed);
      RotateServo(BACK, RIGHTFORWARD);
      delay(100);
      RotateServo(FRONT, RIGHTFORWARD);
      delay(walkSpeed);
#else
      RotateServo(FRONT, LEFTFORWARD);
      delay(walkSpeed);
      RotateServo(BACK, RIGHTFORWARD);
      delay(walkSpeed);
      RotateServo(FRONT, RIGHTFORWARD);
      delay(walkSpeed);
      RotateServo(BACK, LEFTFORWARD);
      delay(walkSpeed);

      RotateServo(FRONT, CENTER);
      delay(100);
      RotateServo(BACK, CENTER);
#endif
      delay(500);
    }

    void moveLeft() {
      Serial.println("Left");
      RotateServo(BACK, 110);
      RotateServo(FRONT, 140);
      delay(100);
      RotateServo(FRONT, CENTER);
      delay(50);
      RotateServo(BACK, CENTER);
      delay(140);
    }

    void moveRight() {
      Serial.println("Right");
      RotateServo(BACK, 70);
      RotateServo(FRONT, 40);
      delay(100);
      RotateServo(FRONT, CENTER);
      delay(50);
      RotateServo(BACK, CENTER);
      delay(140);
    }

    void stepForward() {
      for (int n = 0; n < 4; n++) {
        Serial.println("n: " + String(n));
        RotateServo(FRONT, walkingForward[n * 2]);
        RotateServo(BACK, walkingForward[(n * 2) + 1]);
        delay(STEP_DELAY);
      }

    }


    void RotateServo(uint32_t id, uint32_t angle) {
      //Serial.println("\nStart");
      //setDistance(angle);
      if (m_Distance < 10) {
        delay(10);
        //uint32_t pulseLen = getPulse(0);
        Serial.println("STOOOP" + String(m_Distance));
        moveRight();
      } else {

#if 1
        //Serial.println("Move");
        uint32_t pulse = CalculatePulse(angle);
        m_Pwm.writeMicroseconds(id, pulse);
#endif
      }
    }

};
#endif
