#ifndef STATECONTROLLER_H__
#define STATECONTROLLER_H__

#include "Controller.h"

class StateController() {
  private:
  uint32_t m_Distance;
  Controller m_Controller;
  uint32_t m_State;
  uint32_t m_EscapeDistanceWalk = 40;
  uint32_t m_EscapeDistanceTurn = 60;
  /*
   * 1 == MoveForward
   * 2 == Drehen (vorbereitung)
   * 3 == LinksDrehen
   * 4 == RechtsDrehen
   */
  
  public:
  enum {
    FORWARD     = 1,
    TURN        = 2,
    RIGHTTURN   = 3,
    LEFTTURN
  }
  
  StateController() {
    m_State = 1;
  }
  ~StateController() {
  }

  void checkState() {
    
    uint32_t distance = m_Controller.MeasureDistance();
    if (m_Distance < 60 && m_Distance >40)
    if (m_State = 1 && m_Distance > 40) {
      
    } else if (distance < 40 && m_State = 1) {
      m_State = 2;
    } else if (distance > 60
    
  }
  
};
#endif
