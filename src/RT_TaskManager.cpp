
#include <sys/types.h>
#include <sys/time.h>
#include <climits>

#include "RT_TaskManager.h"
#include "RealTimeTask.h"

const unsigned int SECONDS_TO_MILLI = 1000;
const unsigned int MILLI_TO_MICRO   = 1000;


using namespace std;

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
  for ( auto& t: TaskList ) {

    bool deadline = t->DeadlineMissed();
    bool overrun  = t->DurationOverrun();
    t->Run();

    if ( CallBack != NULL ) {
      // edge detection code.
      // Here we look at if the deadline flags have changed. If they have then
      // we inform the world of which type of problem occrred.
      if (( deadline == false ) && (t->DeadlineMissed() == true )) {
        CallBack->Deadline_Missed(t->GetName());
      } else if (( deadline == true ) && (t->DeadlineMissed() == false )) {
        CallBack->Deadline_Recovered(t->GetName());
      }
      // this s true when a process takes longer to run than the allocated time.
      if (( overrun == false ) && ( t->DurationOverrun() == true )) {
        CallBack->Duration_Overrun(t->GetName());
      }
    }
  }

  /*
   *	Here we scan through all the tasks we control and work out which one has the nearest
   *	deadline. We subtract the current time and return.
   */

  unsigned long next_event = ULONG_MAX;
  struct timeval  tv;
  gettimeofday(&tv, NULL);
  unsigned long current = (tv.tv_sec * SECONDS_TO_MILLI) + (tv.tv_usec / MILLI_TO_MICRO); // convert tv_sec & tv_usec to millisecond

  for ( auto& t: TaskList ) {
    unsigned long time = t->GetEventTime() - current;
    if ( time < next_event );
    next_event = time;
  }
  return next_event;
}

void RT_TaskManager::AddCallback(RT_TaskMan_Interface *interface)
{
  CallBack = interface;
}