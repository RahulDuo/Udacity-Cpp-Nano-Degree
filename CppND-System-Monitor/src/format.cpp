#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  long HH{seconds/3600};
  long H_q{seconds%3600};
  long MM{H_q/60};
  long SS{H_q%60}; 
  string HH_string{std::to_string(HH)};
  string MM_string{std::to_string(MM)};
  string SS_string{std::to_string(SS)};
  string time{HH_string + ':' + MM_string + ':' + SS_string};
  return time ;
 }