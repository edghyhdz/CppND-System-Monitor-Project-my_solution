#include <unistd.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Class constructor
Process::Process(int pid) : pid_(pid) {}  

// DONE: Return this process's ID
int Process::Pid() { return this->pid_; }

// Set function -> sets past_utilization vector for current process
void Process::setPastUtilization(vector<float> current_utilization) {
  this->past_utilization_ = current_utilization;
}

// Mutator function, gets past_utilization
std::vector<float> const Process::getPastUtilization() {
  return this->past_utilization_;
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
  vector<string> cpu_data = LinuxParser::CpuUtilization(this->Pid());
  vector<float> current_cpu_utilization;

  if (cpu_data.empty()){
    return 0; 
  }

  // Based on Vihelm Gray's answer from stackoverflow
  // https://stackoverflow.com/a/16736599/13743493
  float up_time = (float)LinuxParser::UpTime();

  // Make a float vector out of the parsed linux data
  for (string& k : cpu_data) {
    current_cpu_utilization.push_back(stof(k));
  }

  // Calculate total time -> utime + stime + cutime + cstime
  float total_time = current_cpu_utilization[0] + current_cpu_utilization[1];
  total_time = total_time + current_cpu_utilization[2] + current_cpu_utilization[3];

  // Get total elapsed time
  // This is also the total time that process has been running
  this->total_elapsed_time_ =
        up_time - current_cpu_utilization[4] / sysconf(_SC_CLK_TCK);
  // float seconds = up_time - total_elapsed_time_;

  // CPU Usage
  this->cpu_utilization_ = (total_time/sysconf(_SC_CLK_TCK))/this ->total_elapsed_time_;

  return cpu_utilization_;
}


// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(this->pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { 
  return to_string(stoi(LinuxParser::Ram(this->pid_)) / 1024); }

// DONE: Return the user (name) that generated this process
string Process::User() {
  string user = LinuxParser::User(this->pid_);
  return user;
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return this->total_elapsed_time_; }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return this->cpu_utilization_ < a.cpu_utilization_ ? true : false;
}

// Overloaded the "equal to" comparison operator for Process objects
bool Process::operator==(Process const& a) const {
  return this->pid_ == a.pid_ ? true : false;
}