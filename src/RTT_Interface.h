#ifndef __RTT_INTERFACE__
#define __RTT_INTERFACE__

namespace RealTime {

class Task_Interface {

public:
  virtual void Run_Task(void) = 0;

};

}  //  namespace RealTime

#endif
