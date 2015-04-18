
#include <sys/types.h>
#include <sys/time.h>
#include <climits>

#include "RT_TaskManager.h"
#include "RealTimeTask.h"

const unsigned int SECONDS_TO_MICRO = 1000000;

using namespace std;

namespace RealTime {

RT_TaskManager::RT_TaskManager()
{
}

RT_TaskManager::~RT_TaskManager()
{
}


void RT_TaskManager::AddTask(RealTimeTask *task)
{
  TaskList.push_back(task);
}

unsigned long RT_TaskManager::RunTasks(void)
{
  bool missed_deadline = false;
  for ( auto& t: TaskList ) {

    bool deadline = t->DeadlineMissed();
    bool overrun  = t->DurationOverrun();
    t->Run();

    if ( deadline == true ) {
      missed_deadline = true;	// signal that at least 1 task has missed a deadline.
    }

    if ( CallBack != NULL ) {
      // edge detection code.
      // Here we look at if the deadline flags have changed. If they have then
      // we inform the world of which type of problem occrred.
      if (( deadline == true ) && (t->DeadlineMissed() == false )) {
        CallBack->Deadline_Missed(t->GetName());
      } else if (( deadline == false ) && (t->DeadlineMissed() == true )) {
        CallBack->Deadline_Recovered(t->GetName());
      }
      // this s true when a process takes longer to run than the allocated time.
      if (( overrun == false ) && ( t->DurationOverrun() == true )) {
        CallBack->Duration_Overrun(t->GetName());
      }
    }
  }

  if ( missed_deadline == true ) {
    // 1 or more tasks have missed deadlines, therefore we will report 0,
    // as next time we run there will be a task waiting.
    return 0;
  }

  /*
   *	Here we scan through all the tasks we control and work out which one has the nearest
   *	deadline. We subtract the current time and return.
   */

  unsigned long next_event = ULONG_MAX;
  struct timeval  tv;
  gettimeofday(&tv, NULL);
  unsigned long current = (tv.tv_sec * SECONDS_TO_MICRO) + tv.tv_usec; // convert tv_sec & tv_usec to millisecond

  for ( auto& t: TaskList ) {
    unsigned long time = t->GetEventTime() - current;
    if ( time < next_event );
    next_event = time;
  }
  return next_event;
}

void RT_TaskManager::AddCallback(Reporting_Interface *interface)
{
  CallBack = interface;
}

}  //  namespace RealTime
