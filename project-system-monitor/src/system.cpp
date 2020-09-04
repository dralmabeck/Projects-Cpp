#include "system.h"

// DONE - TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE - TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {

  // Initialize empty vector for PID's
  vector<int> pids = LinuxParser::Pids();

  // Loop over all PID's and insert into vector
  if (processes_.size() == 0) {
      for(int pid : pids) {
          processes_.push_back(Process(pid));
      }
  }

  // Sort list of processes according to custom user function:
  // Currently CPU utilization is used, but also Ram or other variables can be used
  sort(processes_.begin(), processes_.end(), std::greater<Process>());

  return processes_;
}

// DONE - TODO: Return the system's kernel identifier (string)
string System::Kernel() { return string(LinuxParser::Kernel()); }

// DONE - TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE - TODO: Return the operating system name
string System::OperatingSystem() { return string(LinuxParser::OperatingSystem()); }

// DONE - TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE - TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE - TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
