#include "process.h"

// DONE - TODO: Return this process's ID
int Process::Pid() const { return pid; }

// DONE - TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 

    // Calculate CPU utilization of single process according to following ressource
    // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    float active_jiffies = (float)LinuxParser::ActiveJiffies(pid);
    float system_uptime = (float)LinuxParser::UpTime();

    float seconds = system_uptime - UpTime() / HERTZ;
    float result = (active_jiffies / HERTZ) / seconds;

    return result;
}

// DONE - TODO: Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(pid); }

// DONE - TODO: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid); }

// DONE - TODO: Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(pid); }

// DONE - TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid); }

// DONE - TODO: Overload the "less than" comparison operator for Process objects
// Sort according to CPU utilization of processes
bool Process::operator<(Process const& a) const { return CpuUtilization() < a.CpuUtilization(); }

 // DONE - TODO: Overload the "more than" comparison operator for Process objects
 // Sort accordign to CPU utilization of processes
bool Process::operator>(Process const& a) const { return CpuUtilization() > a.CpuUtilization(); }