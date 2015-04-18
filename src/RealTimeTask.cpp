// ----------------------------------

#include <sys/time.h>
#include "RealTimeTask.h"

using namespace std;

namespace RealTime {

// ----------------------------------
const unsigned int DEFAULT_PERIOD = 1000000; // micro seconds, 1Hz.
const unsigned int MAX_HERTZ = 1000000;      // largest allowed frequency.
const unsigned int MIN_PERIOD = 1000;        // micro seconds, 1000Hz.

const unsigned int SECONDS_TO_MICRO = 1000000;

// ----------------------------------
RealTimeTask::RealTimeTask(const string & name, Task_Interface * task):
  Name(name), Task(task), DeadlineMissedFlag(false), DurationOverrunFlag(false),
  Duration(MIN_PERIOD), Period(DEFAULT_PERIOD)
{
  SetNextEvent();
}

unsigned long RealTimeTask::GetTime(void)
{
  struct timeval  tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * SECONDS_TO_MICRO) + tv.tv_usec; // convert tv_sec & tv_usec to millisecond
}

void RealTimeTask::SetNextEvent(void)
{
  NextEvent = GetTime();
}

unsigned long RealTimeTask::GetEventTime(void)
{
  return NextEvent;
}

void RealTimeTask::Run(void)
{
  DurationOverrunFlag = false;

  unsigned long current = GetTime();

  if (( current  >= NextEvent ) && ( Task != NULL )) {
    Task->Run_Task();
    NextEvent += Period; // update the base time.

    DurationOverrunFlag = ((GetTime() - current ) >= Duration)? true: false;
  }

  // Missed period detection.
  // Once we update the base timer, it should now be ahead of the current time.
  // If it is not, then we know we have missed a deadline at some point. If this is
  // a transient thing, the module will catch up, otherwise it will continue to fall
  // behind, and action must be taken.
  // Once we drag NextEvent forward, current should be less than it.
  // If it is not, then it is a sign that we have missed at least one deadline.
  DeadlineMissedFlag = ( current > NextEvent )? true: false;
}



/*
 *  When we update the period, there is a chance it will report a missed
 *  deadline based on what the previous Period was. Thus for safety when
 *  we update the Period, we also update the NextEvent variable.
 */

void RealTimeTask::SetPeriod_Us(unsigned int us)
{
  Period = ( us < MIN_PERIOD ) ? MIN_PERIOD: us;
  SetNextEvent();
}

void RealTimeTask::SetPeriod_Ms(unsigned int ms)
{
  SetPeriod_Us(ms * 1000);
}

void RealTimeTask::SetFrequency(unsigned int hz)
{
  if ( hz <= 0 ) {
    Period = DEFAULT_PERIOD;
  } else  if ( hz >= MAX_HERTZ ) {
    Period = MIN_PERIOD;
  } else {
    Period = 1000000 / hz;
  }
  SetNextEvent();
}

void RealTimeTask::SetMaxDuration_Us(unsigned int us)
{
  Duration = us;
}

void RealTimeTask::SetMaxDuration_Ms(unsigned int ms)
{
  Duration = ms * 1000;
}

}  //  namespace RealTime
