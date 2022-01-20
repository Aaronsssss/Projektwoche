#ifndef STATECONTROLLER_H__
#define STATECONTROLLER_H__

#include "Controller.h"
#include <stdlib.h>

class StateController {
  private:
    uint32_t m_Distance;
    //Controller m_Controller;
    uint32_t m_State;
    uint32_t m_EscapeDistanceWalk = 40;
    uint32_t m_EscapeDistanceTurn = 60;

  public:
    enum {
      FORWARD     = 1,
      TURN        = 2,
      RIGHTTURN   = 3,
      LEFTTURN    = 4
    };

    StateController() {
      m_State = 1;
    }
    ~StateController() {
    }

    void checkState() {
      uint32_t distance = m_Controller.MeasureDistance();

      if (m_State = FORWARD && m_Distance < m_EscapeDistanceWalk) {
        m_State = TURN;
      } else if ((m_State == RIGHTTURN || m_State == LEFTTURN) && m_Distance == m_EscapeDistanceTurn) {
        m_State = FORWARD;
      }

      if (m_State == TURN) {
        if ((rand() % 2) != 0) {
          m_State = RIGHTTURN;
          Serial.println("Switch zu Right");
        } else {
          m_State = LEFTTURN;
          Serial.println("Switch zu Left");
        }
      }
    }
    
    void walk() {
      switch (m_State) {
        case FORWARD:
         // m_Controller.stepForward();
          Serial.println("Step Forward");
          break;

        case RIGHTTURN:
         // m_Controller.moveRight();
          Serial.println("Move Right");
          break;

        case LEFTTURN:
         // m_Controller.moveLeft();
          Serial.println("Move Left");
          break;

        default:
          break;
      }
    }

};
#endif
