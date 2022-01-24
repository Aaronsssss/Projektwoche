#ifndef STATECONTROLLER_H__
#define STATECONTROLLER_H__

#include "Controller.h"

class StateController 
{
  private:
  uint32_t m_Distance;
  Controller *m_Controller;
  uint32_t m_State;
  uint32_t m_WalkDirection;
  uint32_t m_TurnCount;
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
    STM_CHOOSE_DIRECTION,
    
    ACTION_MOVE_FORWARD,
    TURN_UNTIL_FAILTURE = 30,
  };
  
  StateController() : m_WalkDirection(0)
  {
    m_State = STM_MOVE_CENTER;
    m_Working = true;
    m_Controller = new Controller();
    m_Controller->Init();
   // Serial.println("Init done of StateController");
  }
  ~StateController()  
  {
    if(m_Controller)
    {
      delete m_Controller;
      m_Controller = NULL;
    }
  }

  void StateMachine() 
  {
    uint32_t distanceResult;
    bool validResult = false;
    //m_Controller->testMove();
      switch(m_State)
      {
        default: /// fallthough
        
        case STM_MOVE_CENTER:
        #if 1
          m_Controller->RotateServo(Controller::FRONT,Controller::CENTER);
          m_Controller->RotateServo(Controller::BACK,Controller::CENTER);


        #else
          
          delay(2000);
          m_Controller->RotateServo(Controller::FRONT,0);
          m_Controller->RotateServo(Controller::BACK,0);
          delay(2000);
          m_Controller->RotateServo(Controller::FRONT,180);
          m_Controller->RotateServo(Controller::BACK,180);
          delay(2000);
          #endif
          m_State = STM_MEASURE_DISTANCE;
          //m_State = STM_MOVE_CENTER;
          break;
          #if 0
        case STM_MOVE_LEFT:
          break;
        case STM_MOVE_RIGHT:
          break;
        case STM_MOVE_FORWARD:
          break;
#endif
        case STM_MOVE_BACKWARD:
          break;
          case STM_MEASURE_DISTANCE:
          
            validResult = m_Controller->MeasureDistance();
            if(validResult)
            {
              m_State = STM_VALIDATE_DISTANCE;
              validResult = false;
            }
          break;
        case STM_VALIDATE_DISTANCE:
          distanceResult = m_Controller->getDistance();
          Serial.println("Hyst result: " + String(distanceResult));
          if(Controller::DISTANCE_SHORT == distanceResult)
          {
            m_State = STM_CHOOSE_DIRECTION;
          }
          else if (Controller::DISTANCE_FAR == distanceResult)
          {
            m_Controller->SetMovementDirection(ACTION_MOVE_FORWARD);
             
            Serial.println("Move Forward");
            m_Controller->stepForward();
            m_State = STM_MEASURE_DISTANCE;
          }
          
          break;
        
        case STM_CHOOSE_DIRECTION:
          if (0 == m_WalkDirection)
          {
            m_WalkDirection = rand() % 2 + 1;
            Serial.println("New Walking direction");
            
          }
          else
          {
            m_TurnCount++;
            
            if(m_TurnCount > TURN_UNTIL_FAILTURE)
            {
              Serial.println("Generate oposite walking dir");
              m_TurnCount = 0;
              m_WalkDirection = 0;//3 - m_WalkDirection;
            }
          }
          //Serial.println("m_WalkDirection: " + String(m_WalkDirection)+" TurnCount: " + String(m_TurnCount));
          m_Controller->Turn(m_WalkDirection);
          m_State = STM_MEASURE_DISTANCE;
          break;
      
    }
    
  
  }
};
#endif
