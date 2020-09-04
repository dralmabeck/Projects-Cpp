#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

// This factor converts from system ticks to seconds
#define HERTZ (float)sysconf(_SC_CLK_TCK)

#include <fstream>
#include <regex>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <cctype>
#include <sstream>
#include <cstddef>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::istringstream;
using std::ifstream;
using std::replace;
using std::getline;
using std::stol;
using std::stoi;

namespace LinuxParser {

    // Paths
    const string kProcDirectory{"/proc/"};
    const string kCmdlineFilename{"/comm"};
    const string kStatusFilename{"/status"};
    const string kStatFilename{"/stat"};
    const string kUptimeFilename{"/uptime"};
    const string kMeminfoFilename{"/meminfo"};
    const string kVersionFilename{"/version"};
    const string kOSPath{"/etc/os-release"};
    const string kPasswordPath{"/etc/passwd"};

    // System
    float MemoryUtilization();
    long UpTime();
    vector<int> Pids();
    int TotalProcesses();
    int RunningProcesses();
    string OperatingSystem();
    string Kernel();

    // CPU
    enum CPUStates {
        kUser_ = 0,
        kNice_,
        kSystem_,
        kIdle_,
        kIOwait_,
        kIRQ_,
        kSoftIRQ_,
        kSteal_,
        kGuest_,
        kGuestNice_
    };

    vector<string> CpuUtilization();
    long ActiveJiffies(int pid);

    // Processes
    string Command(int pid);
    string Ram(int pid);
    string Uid(int pid);
    string User(int pid);
    long int UpTime(int pid);
};  // namespace LinuxParser

#endif