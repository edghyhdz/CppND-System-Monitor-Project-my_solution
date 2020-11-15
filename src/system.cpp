#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

bool Compare(Process& a, Process& b) { return a < b; }

// setter, used to set previous_process_ member variable
// Used to calculate current CPU's usage
void System::setPreviousProcesses(
    std::vector<Process> processes) {
  this->previous_processes_ = processes;
}

// Updates processes_ vector
void System::CheckRunningProcesses() {
  bool found_process{false};
  vector<int> parsed_pids = LinuxParser::Pids(); 
  // Getting previous time stemp for later comparison
  // in calculating CPU utilization
  setPreviousProcesses(processes_);

  // Checks from linuxparser pids if pid is already inside vector
  for (int pid: parsed_pids){
    found_process = false;
    for (Process& a : this->processes_){
      if (a.Pid() == pid){
        found_process = true; 
      }
    }
    // If process not found, add it into processes vector
    if (!found_process){
      Process new_pid(pid); 
      processes_.push_back(new_pid);
    }
  }

  // Drop processes that are not running anymore
  for (std::size_t i = 0; i < processes_.size(); i++){
    found_process = false;
    for (int pid : parsed_pids){
       if (processes_[i] == pid){
         found_process = true;
       }
    }
    // Deletes no longer running processes
    if (!found_process){
      processes_.erase(processes_.begin() + i); 
    }
  }
}

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {

  // Check for running processes before updating processes_ vector
  System::CheckRunningProcesses();
  
  for (Process& p : processes_){
    p.CpuUtilization();
  }

  std::sort(processes_.begin(), processes_.end(), Compare);
  std::reverse(processes_.begin(), processes_.end());

  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }