// constants won't change. They're used here to set pin numbers:
const int SENSOR_PIN = 8; // the Arduino's input pin that connects to the sensor's SIGNAL pin 
const int SENSOR_PIN_MAX_DELAY = 300;
const int PROGRAMM_DELAY = 100;
const int red_light_pin = 3;
const int green_light_pin = 5;
const int blue_light_pin = 6;
// Variables will change:
int lastState = LOW;      // the previous state from the input pin
int currentState;         // the current reading from the input pin
uint32_t timestamp = 0;
uint32_t timeNow = 0;
uint32_t timeDif = 0;
uint32_t selector = 0;
uint32_t counter = 0;
typedef struct
{
  uint32_t r,g,b;
}Color_t;
Color_t colors[10];
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the Arduino's pin as aninput
  pinMode(SENSOR_PIN, INPUT);
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  colors[0] = {255, 0, 0};
  colors[1] = {0, 255, 0};
  colors[2] = {0, 0, 255};
  colors[3] = {255, 255, 125};
  colors[4] = {0, 255, 255};
  colors[5] = {255, 0, 255};
  
}


void loop() {
  RGB_color(2);
  #if 0
 
  delay(PROGRAMM_DELAY);
  // read the state of the the input pin:
  currentState = digitalRead(SENSOR_PIN);
  
  if(lastState == LOW && currentState == HIGH || lastState == HIGH && currentState == LOW)//
  {
    counter++;
    
    timeNow = millis();

  timeDif = timeNow - timestamp;
  timestamp = timeNow;
    uint32_t resetTime = SENSOR_PIN_MAX_DELAY + PROGRAMM_DELAY -10;
    if(timeDif < resetTime )
    {
      counter = 0;
      
    }
    Serial.println("Selected value is" + String(counter)+ " Time dif: "+ String(timeDif)+" Reset time: "+ String(resetTime));
 RGB_color(counter);
  }
  // save the the last state
  
  lastState = currentState;
  #endif
}
void RGB_color(uint32_t id)
 {
  //Serial.println("ID: "+String(id));
  Color_t color = colors[id];
 // if(color)
  //{
    //Serial.println("ID: "+String(id));
    analogWrite(red_light_pin, color.r);
    analogWrite(green_light_pin, color.g);
    analogWrite(blue_light_pin, color.b);
 // }
}
