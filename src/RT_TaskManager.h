#ifndef __RT_TASKMANAGER__
#define __RT_TASKMANAGER__

#include <vector>
#include <string>

namespace RealTime {

class RealTimeTask;

class Reporting_Interface {
public:
  virtual void Deadline_Missed(const std::string & name) = 0;
  virtual void Deadline_Recovered(const std::string & name) = 0;
  virtual void Duration_Overrun(const std::string & name) = 0;
};

class RT_TaskManager {
public:
  RT_TaskManager();
  ~RT_TaskManager();

  void AddTask(RealTimeTask *task);
  void AddCallback(Reporting_Interface *interface);
  unsigned long RunTasks(void);

private:
  std::vector<RealTimeTask *> TaskList;
  Reporting_Interface *CallBack;

  RT_TaskManager& operator=(RT_TaskManager other);
};

}  //  namespace RealTime

#endif
