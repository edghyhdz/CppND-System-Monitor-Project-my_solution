#include "processor.h"
#include "linux_parser.h"

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpu_utilization = LinuxParser::CpuUtilization();
  vector<float> past_utilization = Processor::getPastUtilization();

  // Using Vangelis Tasoulas stackoverflow reference
  // https://stackoverflow.com/a/23376195/13743493
  // Calculate first utilization
  if (!past_utilization.empty()) {
    float prev_idle = past_utilization[3] + past_utilization[4];
    float idle = stof(cpu_utilization[3]) + stof(cpu_utilization[4]);

    // Calculate non idle
    float prev_non_idle = 0, non_idle = 0;
    for (int i : {0, 1, 2, 5, 6, 7}) {
      prev_non_idle += past_utilization[i];
      non_idle += stof(cpu_utilization[i]);
    }

    // Calculate totals
    float prev_total = prev_idle + prev_non_idle;
    float total = idle + non_idle;

    // Setting current utilization as past utilization
    vector<float> current_cpu_utilization = convertToFloat(cpu_utilization);
    Processor::setPastUtilization(current_cpu_utilization);

    float total_dif = total - prev_total;
    float idle_dif = idle - prev_idle; 
    return (total_dif - idle_dif)/ total_dif;

  } else {
    // Setting current utilization as past utilization
    vector<float> current_cpu_utilization = convertToFloat(cpu_utilization);
    Processor::setPastUtilization(current_cpu_utilization);
    
    // Return an initilaization value
    return 0.0;
  }

}

// Saves 't-1' cpu utilization vector, to be used in at time 't'
// to calculate cpu usage
void Processor::setPastUtilization(std::vector<float> &past_utilization) {
  this->past_utilization_ = past_utilization;
}

// Mutator function, gets past_utilization
std::vector<float> const Processor::getPastUtilization() {
  return this->past_utilization_;
}

// Converts vector of strings into vector of floats
vector<float> Processor::convertToFloat(vector<string> &v) {
    vector<float> current_cpu_utilization;
    for (string &k : v) {
      current_cpu_utilization.push_back(stof(k));
    }
    return current_cpu_utilization; 
}