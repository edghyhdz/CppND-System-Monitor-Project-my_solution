#include <dirent.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string row_name;
  float memory;
  vector<float> memory_vector;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {
    // We just need the first 4 rows
    for (int i = 0; i < 4; i++) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> row_name >> memory;
      memory_vector.push_back(memory);
    }
    // From Hisham H M's answer https://stackoverflow.com/a/41251290/13743493
    // TODO: Add buffers, and cached memory
    return (memory_vector[0] - memory_vector[1]) /
           memory_vector[0];  // MemTotal - MemFree
  }
  return 0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long int suspend, idle, total_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> suspend >> idle;
    total_time = suspend + idle;
  }

  return total_time;
}

// NOTUSED -> Parsing all at once into single vector using
// CpuUtilization() member function, either for system or single processes
// NOTUSED: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// NOTUSED: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// NOTUSED: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, temp_cpu;
  vector<string> cpu_utilization;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> temp_cpu) {
      if (i > 0) {
        cpu_utilization.push_back(temp_cpu);
      }
      i++;
    }
  }
  return cpu_utilization;
}

// Parses Jiffies 
vector<string> LinuxParser::CpuUtilization(int pid) {
  string line, temp_cpu;
  // Positions in vector where relevant info is
  vector<int> v_check{13, 14, 15, 16, 21};  
  vector<string> cpu_utilization;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> temp_cpu) {
      // if i is in v_check, it means its info we want to parse
      if (std::find(v_check.begin(), v_check.end(), i) != v_check.end()) {
        cpu_utilization.push_back(temp_cpu);
      }
      i++;
    }
  }
  return cpu_utilization;
}

// Helper function for TotalProcesses() and RunningProcesses() functions
int LinuxParser::ProcessesFinder(string str_search) {
  string line;
  string row_name;
  int variable;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      // refference for string::npos
      // https://stackoverflow.com/a/2340309/13743493
      if (line.find(str_search) != string::npos) {
        std::istringstream linestream(line);
        linestream >> row_name >> variable;
      }
    }
  }
  return variable;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return LinuxParser::ProcessesFinder("processes");
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return LinuxParser::ProcessesFinder("procs_running");
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmd;

  std::ifstream stream(kProcDirectory + "/" + to_string(pid) +
                       kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmd);
  }
  return cmd;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, vm_size, ram;

  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream streamline(line);
      streamline >> vm_size >> ram;
      if ("VmSize:" == vm_size){
        break;
      }
    }
  }
  return ram; }

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, user, uid_title;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream streamline(line);
      streamline >> uid_title >> user;
      if (uid_title == "Uid:") {
        break;
      }
    }
  }
  return user;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, user_name, x, user_check;
  string user_number = LinuxParser::Uid(pid);

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user_name >> x >> user_check;
      if (user_check == user_number) {
        return user_name;
      }
    }
  }
  return string();
}