#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

// This factor converts from system ticks to seconds
#define HERTZ (static_cast<double>(sysconf(_SC_CLK_TCK)))

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

using std::string;
using std::to_string;
using std::vector;
using std::istringstream;
using std::ifstream;
using std::replace;
using std::getline;
using std::stol;
using std::stoi;

// Custom string to long function to handle errors when function arguments are invalid
long mystol(string input);

namespace LinuxParser {

    // Paths
    const string kProcDirectory{"/proc/"};
    const string kCmdlineFilename{"/cmdline"};
    const string kStatusFilename{"/status"};
    const string kStatFilename{"/stat"};
    const string kUptimeFilename{"/uptime"};
    const string kMeminfoFilename{"/meminfo"};
    const string kVersionFilename{"/version"};
    const string kOSPath{"/etc/os-release"};
    const string kPasswordPath{"/etc/passwd"};

    // String filters
    const string filterProcesses("processes");
    const string filterRunningProcesses("procs_running");
    const string filterMemTotalString("MemTotal:");
    const string filterMemFreeString("MemFree:");
    const string filterCpu("cpu");
    const string filterUID("Uid:");

    // Here I am using VmData instead of VmSize as suggested by the first reviewer
    // This is a deviation from Udacity Guidelines, but I want to show the physikcal Ram size
    // VmSize would give me total Memory size (can exceed physical)
    const string filterProcMem("VmData:");

    // System
    double MemoryUtilization();
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