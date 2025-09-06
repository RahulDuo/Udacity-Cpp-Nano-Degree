#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float total_jiffies_{0.0}, total_jiffies_prev_{0.0}, idle_jiffies_{0.0}, util_jiffies_{0.0}, util_jiffies_prev_{0.0}, usage_{0.0};
};

#endif