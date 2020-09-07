#include "process.h"

// DONE - TODO: Return this process's ID
int Process::Pid() const { return pid; }

// DONE - TODO: Return this process's CPU utilization
double Process::CpuUtilization() const { 

    // Calculate CPU utilization of single process according to following ressource
    // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    double active_jiffies = static_cast<double>(LinuxParser::ActiveJiffies(pid));
    double system_uptime = static_cast<double>(LinuxParser::UpTime());

    double seconds = system_uptime - UpTime() / HERTZ;

    // We are adding one second to keep denominator numerically stable
    double result = (active_jiffies / HERTZ) / (seconds + 1.0);

    return result;
}

// DONE - TODO: Return the command that generated this process
string Process::Command() const {
    string command = LinuxParser::Command(pid);

    if(command.size() > 40) {
        command.resize(37);
        command = command + "...";
    }
    return command;
}

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