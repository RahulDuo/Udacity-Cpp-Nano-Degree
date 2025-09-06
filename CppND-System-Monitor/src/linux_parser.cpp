#include <dirent.h>
#include <unistd.h>
#include <sstream>
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
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float totalMemory , freeMemory;
  string line,key,value,kb;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
    	std::istringstream linestream(line);
      while(linestream >> key >>value >>kb){
        if(key=="MemTotal:"){totalMemory = std::stof(value);}
        if(key=="MemFree:"){freeMemory = std::stof(value);}
      }
    }
  	
  }
  
return totalMemory > 0.0f ? (totalMemory - freeMemory) / totalMemory : 0.0f;

}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string uptime_,idletime,line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_ >> idletime;
	uptime = std::stol(uptime_);
}
  return uptime;
}
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0;}

// TODO  Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) {
  long totalClk;
  string value,line;
  vector<string> statList;
  std::ifstream stream(kProcDirectory +std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value){
    	statList.push_back(value);
    }
  }
  
  long int utime{std::stol(statList[13])};
  long int stime{std::stol(statList[14])};
  long int cutime{std::stol(statList[15])};
  long int cstime{std::stol(statList[16])};
  totalClk = utime+stime+cutime+cstime;
  return totalClk;
  
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string value, line, key;
  vector<string> jiffies_list;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  	std::istringstream linestream(line);
   	while (linestream >> key){
    	if (key != "cpu"){
          jiffies_list.push_back(key);}
    }
    }
  return jiffies_list; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string value, line, key;
  int val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
    	if (key == "processes"){
          val = std::stoi(value);
        return val;
        }
    }
  		
    }
    }
  return 0;

}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
   string value, line, key;
  int val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
    	if (key == "procs_running"){
          val = std::stoi(value);
        return val;
        }
    }
  		
    }
    }
  return 0;

}


// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    
    if (std::getline(stream, line)) {
      return line;
    }
  }
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string key,value,line;
  int vmsize;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
  	while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
    	if (key == "VmSize:"){
          vmsize = std::stoi(value);
          vmsize = vmsize/1000;
          return std::to_string(vmsize);
    
    }
  }
    }
  }
  
  return "404"; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string key,value,uid,line;
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
  	while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
    	if (key == "Uid:"){
          uid = value;
    
    }
  }
    }
  }
  
  return uid; }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string user,password,uid,line;
  string uid_ = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
      while(linestream>>user>>password>>uid){
        if (uid == uid_){
          return user;
        }
      
      }
    }
  
  }
  
  
  return "404"; }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string value, line;
  long int starttime, uptime;
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
			stat_list.push_back(value);
        }
    }
  starttime = std::stol(stat_list[21])/sysconf(_SC_CLK_TCK);
  uptime =  LinuxParser::UpTime() - starttime;
  return uptime;
}
