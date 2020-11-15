#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <string>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  std::vector<float> const getPastUtilization();
  void setPastUtilization(std::vector<float> &past_utilization);
  std::vector<float> convertToFloat(std::vector<std::string> &v);
  
  // TODO: Declare any necessary private members
 private:
  std::vector<float> past_utilization_;
};

#endif