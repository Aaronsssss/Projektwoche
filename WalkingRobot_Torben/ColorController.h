#ifndef COLORCONTROLLER_H__
#define COLORCONTROLLER_H__

#define LED_RED 3
#define LED_GREEN 5
#define LED_BLUE 6
class ColorController 
{
  public: 
  typedef struct
{
  uint32_t r,g,b;
}Color_t;
  private:
  
  Color_t colors[10];
  public:
  
  

  ColorController()
  {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    colors[0] = {255, 0, 0};
    colors[1] = {130, 255, 0};
    colors[2] = {0, 0, 255};
    colors[3] = {255, 255, 125};
    colors[4] = {0, 255, 255};
    colors[5] = {255, 0, 255};
  }
  ~ColorController()  
  {
   
  }
  void setColor(uint32_t id)
 {
  //Serial.println("ID: "+String(id));
  Color_t color = colors[id];
 // if(color)
  //{
    //Serial.println("ID: "+String(id));
    analogWrite(LED_RED, color.r);
    analogWrite(LED_GREEN, color.g);
    analogWrite(LED_BLUE, color.b);
 // }
}
  
  
  

  

  
};
#endif
