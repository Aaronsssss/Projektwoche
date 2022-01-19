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



//#include <String.h>
class Controller {  
  public:
  enum {
    FRONT        =  14,
    BACK         =  15,
    LEFTFORWARD  = 110,
    CENTER       =  90,
    RIGHTFORWARD =  70,
    SERVO_BACK_DISTANCE = 60,
    SERVO_FORWARD_DISTANCE = 100,
    SERVO_CENTRED = 80,
    
    
  };
  
  typedef double float64_t;
  
  Controller() {
    
    Init();
  }
  ~Controller(){
    
  }
  
  
  void Init() {
	  
    Serial.begin(9600);
    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    m_Pwm = Adafruit_PWMServoDriver();
    m_Pwm.begin();
    //m_Pwm.setOscillatorFrequency(27000000);
    m_Pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
    
    m_Movements[0] = SERVO_BACK_DISTANCE;
    m_Movements[1] = SERVO_FORWARD_DISTANCE;
    m_Movements[2] = SERVO_FORWARD_DISTANCE;
    m_Movements[3] = SERVO_FORWARD_DISTANCE;
    m_Movements[4] = SERVO_FORWARD_DISTANCE;
    m_Movements[5] = SERVO_BACK_DISTANCE;
    m_Movements[6] = SERVO_BACK_DISTANCE;
    m_Movements[7] = SERVO_BACK_DISTANCE;
    Serial.println("Init done\n");
  }
  
  
  String getDistance() {
    return String(m_Distance[0]);
  }
  
  
  void MeasureDistance(){
    uint32_t distance = 0;
    digitalWrite(PIN_TRIGGER, LOW);
    delayMicroseconds(2);
    
    digitalWrite(PIN_TRIGGER, HIGH);
    delayMicroseconds(10);
    
    uint32_t duration = pulseIn(PIN_ECHO, HIGH);
    //Serial.println("duration: "+ String(duration));
    distance = (duration/58.0);
    
    if ( ( distance < SENSOR_MAX_RANGE ) || (distance > 0 ) ) {
      m_Distance[0] = distance;
    }
  }
  
  
  uint32_t CalculatePulse(uint32_t angle) {
    uint32_t rv = angle*11.1+500;

    //Serial.println("Pulse: "+ String(rv));
    return rv;
  }
  
  
  uint32_t checkDirection() {
    uint32_t rv = 0;
    Serial.println("left: "+ String( m_Distance[1])+" center: "+ String( m_Distance[2])+" right: "+ String( m_Distance[3]));


    return rv;
  }
  
  
  void setDistance(uint32_t angle) {
    switch(angle) {
      case LEFTFORWARD:
        m_Distance[1] = m_Distance[0];
        break;
      case CENTER:
        m_Distance[2] = m_Distance[0];
        checkDirection();
        break;
      case RIGHTFORWARD:
        m_Distance[3] = m_Distance[0];
          break;
          
      default:
        break;
      
    }
  }
  void stepForward() {
  for (int n = 0; n < 4; n++) {
    RotateServo(FRONT,m_Movements[n * 2]);
    RotateServo(BACK,m_Movements[(n * 2) + 1]);
    delay(160);
  }
}
  
  void moveForward(){
    uint32_t walkSpeed = 50;
    #if 1
    RotateServo(FRONT,CENTER);
    RotateServo(BACK,CENTER);
    delay(400);
    RotateServo(BACK,LEFTFORWARD);
    delay(100);
    RotateServo(FRONT,LEFTFORWARD);
    delay(walkSpeed);
    RotateServo(BACK,RIGHTFORWARD);
    delay(100);
    RotateServo(FRONT,RIGHTFORWARD);
    delay(walkSpeed);
    #else
    RotateServo(FRONT,LEFTFORWARD);
    delay(walkSpeed);
    RotateServo(BACK,RIGHTFORWARD);
    delay(walkSpeed);
    RotateServo(FRONT,RIGHTFORWARD);
    delay(walkSpeed);
    RotateServo(BACK,LEFTFORWARD);
    delay(walkSpeed);
    
    
    RotateServo(FRONT,CENTER);
    delay(100);
    RotateServo(BACK,CENTER);
    #endif
    MeasureDistance(); // Update Distance
    delay(500);
  }
  
  
  void RotateServo(uint32_t id, uint32_t angle){
     //Serial.println("\nStart");
    MeasureDistance(); // needed? 
    setDistance(angle);
    if(m_Distance[0] < 10){
      
      //uint32_t pulseLen = getPulse(0);
      Serial.println("STOOOP"+String(m_Distance[2]));
      
    } else {
      
      #if 1
      //Serial.println("Move");
        uint32_t pulse = CalculatePulse(angle);
        m_Pwm.writeMicroseconds(id, pulse);

        
     
      #else
      for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        m_Pwm.waitMicroSeconds(id, 0, pulselen);
        //m_Pwm.setPWM(1, 0, SERVOMAX - pulselen);
        //Serial.println("First: " + String(pulselen)+" Second: "+ String(SERVOMAX - pulselen));
      }
    
      delay(2000);
      for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        m_Pwm.setPWM(id, 0, pulselen );
        //m_Pwm.setPWM(1, 0, pulselen );
        //Serial.println("First: "+String(SERVOMAX - pulselen)+" Second: "+ String(pulselen));
      }
      delay(2000);
      #endif
    }
  }
  
  
  #if 0
  uint32_t getPulse(uint32_t state){
    uint32_t rv = 0;
    switch(state) {
      case 0:
        rv = SERVOMIN;
        break;
      case 1:
        rv = SERVOMAX;
        break;
      default:
        break;
    }
  }
  #endif
  

  
  
  
  
  
  private:
  uint32_t m_Distance[4];
  Adafruit_PWMServoDriver m_Pwm;
  uint32_t m_Movements[8];
  
};
#endif
