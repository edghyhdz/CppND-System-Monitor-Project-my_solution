#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>


/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);                        // Added
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator==(Process const& a) const; // To check if previous processes matrix contains current ones

  std::vector<float> const getPastUtilization();
  void setPastUtilization(std::vector<float> current_utilization);

  // TODO: Declare any necessary private members
 private:
  int pid_{0}; 
  float cpu_utilization_{0};
  float total_elapsed_time_{0};
  std::vector<float> past_utilization_;
};

#endif