#include "processor.h"
#include "linux_parser.h"
#include "process.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  auto jiffies_list = LinuxParser::CpuUtilization();

  
  for (size_t i = 0; i < jiffies_list.size(); ++i) {
    float val = std::stof(jiffies_list[i]);
  

    if (i != LinuxParser::kGuest_ && i != LinuxParser::kGuestNice_) {
      total_jiffies_ += val;
    }
  
    if (i == LinuxParser::kIdle_ || i == LinuxParser::kIOwait_) {
      idle_jiffies_ += val;
    }
  }


  util_jiffies_ = total_jiffies_ - idle_jiffies_;
  usage_ = (util_jiffies_ - util_jiffies_prev_) 
         / (total_jiffies_ - total_jiffies_prev_);

  
  total_jiffies_prev_ = total_jiffies_;
  util_jiffies_prev_  = util_jiffies_;

  return usage_;
}

