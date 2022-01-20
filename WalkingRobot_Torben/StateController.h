

#ifndef STATECONTROLLER_H__
#define STATECONTROLLER_H__

#include "Controller.h"

class StateController 
{
  private:
  uint32_t m_Distance;
  Controller m_Controller;
  uint32_t m_State;
  uint32_t m_EscapeDistanceWalk = 40;
  uint32_t m_EscapeDistanceTurn = 60;
  bool m_Working;
  
  public:
  enum {
    STM_MOVE_CENTER,
    STM_MOVE_LEFT,
    STM_MOVE_RIGHT,
    STM_MOVE_FORWARD,
    STM_MOVE_BACKWARD,
    STM_MEASURE_DISTANCE,
    STM_VALIDATE_DISTANCE,
    STM_CHOOSE_DIRECTION
    
  };
  
  StateController() 
  {
    m_State = STM_MOVE_CENTER;
    m_Working = true;
    Serial.println("Init done of StateController");
  StateMachine();
  }
  ~StateController() 
  {
  }

  void StateMachine() 
  {
    uint32_t localDistance;
    while(m_Working)
    {
    
      switch(m_State)
      {
        default: /// fallthough
        
        case STM_MOVE_CENTER:
          m_Controller.RotateServo(Controller::FRONT,Controller::CENTER);
          m_Controller.RotateServo(Controller::BACK,Controller::CENTER);
          m_State = STM_MEASURE_DISTANCE;
          break;
        case STM_MOVE_LEFT:
          break;
        case STM_MOVE_RIGHT:
          break;
        case STM_MOVE_FORWARD:
          break;

        case STM_MOVE_BACKWARD:
          break;
        case STM_MEASURE_DISTANCE:
          m_Controller.MeasureDistance();
          
         // m_State =  STM_VALIDATE_DISTANCE;
          break;
        case STM_VALIDATE_DISTANCE:
          break;
        case STM_CHOOSE_DIRECTION:
          break;
      
    }
    
  }
  }
};
#endif
