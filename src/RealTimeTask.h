#ifndef __REAL_TIME_TASK__
#define __REAL_TIME_TASK__

/***
	RealTimeTaskManager

	A container for managing how often a module should be run.
	Currently offers millisecond resolution.
	Capable of reporting if a deadline has been missed.
	Each task will be run once per period.
	It might be useful to add a function that will return
	how long until the start of the next period.
 */

#include "RTT_Interface.h"
#include <string>

namespace RealTime {

class RealTimeTask {

public:

  RealTimeTask(const std::string & name, Task_Interface * task);

  typedef struct {
    unsigned int Min;    // Shortest runtime of this function
    unsigned int Max;    // Longest runtime of this funtion
    unsigned int Avg;    // Average runtime of this function
    unsigned int Called; // How many times this task has been run.
  } Statistics_t;

  /*
   * SetFrequency()/SetPeriod()
   *
   * Calculates how often this task should be run.
   * SetFrequency()
   * Operates in hertz, useful for tasks that should be run quickly. Min 1Hz, Max 1000Hz.
   *
   * ms - Set the duration in milli seconds
   * us - Set the duration in micro seconds
   *
   * SetPeriod()
   * Sets a period of X ms. Useful for slower tasks. Min 1ms.
   */
  void SetPeriod_Ms(unsigned int ms);
  void SetPeriod_Us(unsigned int us);
  void SetFrequency(unsigned int hz);

  /*
   * SetMaxDuration()
   *
   * Informs the system how  long this task is given to run.
   * If task takes longer than  this time to execute a flag is set.
   *
   * ms - Set the duration in milli seconds
   * us - Set the duration in micro seconds
   */
  void SetMaxDuration_Ms(unsigned int ms);
  void SetMaxDuration_Us(unsigned int ms);

  /*
   * Run()
   * Called at full rate.
   * This function will internally decide when the underlying module should be run.
   */
  void Run(void);

  /*
   * GetEventTime()
   * Returns in milliseconds the time until the next event windows begins.
   */
  unsigned long GetEventTime();

  /*
   *  Returns 1 on the first edge of
   */
  bool DeadlineMissed()  {
    return DeadlineMissedFlag;
  }
  bool DurationOverrun() {
    return DurationOverrunFlag;
  }
  const std::string & GetName(void) {
    return Name;
  }

  Statistics_t GetStats(void) {
    return Stats;
  }

private:
  std::string Name;
  Task_Interface * Task;
  bool DeadlineMissedFlag;
  bool DurationOverrunFlag;

  unsigned int Duration;
  unsigned int Period;
  unsigned long NextEvent;

  void SetNextEvent(void);
  unsigned long GetTime(void);

  Statistics_t Stats;


  RealTimeTask& operator=(RealTimeTask other);
};

}  //  namespace RealTime
#endif
