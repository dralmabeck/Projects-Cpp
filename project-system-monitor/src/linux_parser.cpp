#include "linux_parser.h"

// Custom string to long function to handle errors when function arguments are invalid
long mystol(string input) {
    long result;
    try {
        result = stol(input);
    }
    catch(...) {
        result = 0;
    }
    return result;
}

// Use of template suggested by first reviewer
template <typename T>
T findValueByKey(string const &keyFilter, string const &filename) {

    string line, key;
    T value;

    ifstream stream(LinuxParser::kProcDirectory + filename);
    if (stream.is_open()) {
        while (getline(stream, line)) {
            istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == keyFilter) {
                    return value;
                }
            }
        }
    }
  return value;
};

// Use of template suggested by first reviewer
template <typename T>
T getValueOfFile(string const &filename) {

    string line;
    T value;

    ifstream stream(LinuxParser::kProcDirectory + filename);
    if (stream.is_open()) {
        getline(stream, line);
        istringstream linestream(line);
        linestream >> value;
    }

    return value;
};

// Use of template suggested by first reviewer
template <typename T>
T getContentOfLine(string const &filename) {

    string line;

    ifstream stream(LinuxParser::kProcDirectory + filename);
    if (stream.is_open()) {
        getline(stream, line);
    }

    return line;
};

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream stream(kOSPath);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return to_string(0);
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {

  string os, version, kernel;
  string line = getContentOfLine<string>(kVersionFilename);

  istringstream linestream(line);
  linestream >> os >> version >> kernel;

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
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE - TODO: Read and return the system memory utilization
// Read and return the system memory utilization
double LinuxParser::MemoryUtilization() {

    double Total = findValueByKey<double>(filterMemTotalString, kMeminfoFilename);
    double Free = findValueByKey<double>(filterMemFreeString, kMeminfoFilename);
    return ((Total - Free) / Total);
}

// DONE - TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    return getValueOfFile<long>(kUptimeFilename);
}

// DONE - TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 

    vector<string> jiffies;
    long result;
    string key;
    string line = getContentOfLine<string>(to_string(pid) + kStatFilename);
    istringstream linestream(line);

    try {
        while (linestream >> key) {
            jiffies.emplace_back(key);
        }
        result = (mystol(jiffies[13]) + mystol(jiffies[14]) + mystol(jiffies[15]) + mystol(jiffies[16]));
    }
    catch (...) {
        result = 0;
    }

    return result;
}

// DONE - TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 

    string line = getContentOfLine<string>(kStatFilename);
    vector<string> result;
    istringstream linestream(line);
    string key;

    while (linestream >> key) {
        if (key != filterCpu) {
            result.emplace_back(key);
        }
    }
    return result;
}

// DONE - TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 

    return findValueByKey<int>(filterProcesses, kStatFilename);
}

// DONE - TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {

    return findValueByKey<int>(filterRunningProcesses, kStatFilename);
}

// DONE - TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 

    return string(getValueOfFile<string>(to_string(pid) + kCmdlineFilename));
}

// DONE - TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {

    // Here I am using VmData instead of VmSize as suggested by the first reviewer
    // This is a deviation from Udacity Guidelines, but I want to show the physikcal Ram size
    // VmSize would give me total Virtual Memory size (can exceed physical)
    long result = findValueByKey<long>(filterProcMem, (to_string(pid)+kStatusFilename));
    result = result / 1024;

    return to_string(result);
}

// DONE - TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 

    return findValueByKey<string>(filterUID, (to_string(pid)+kStatusFilename));
}

// DONE - TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) { 

    string line, key, dummy, result;
    string uid = Uid(pid);
    ifstream stream(kPasswordPath);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            replace(line.begin(), line.end(), ':', ' ');
            istringstream linestream(line);
            while (linestream >> result >> dummy >> key) {
                if (key == uid ) {
                    return result;
                }
            }
        }
    }

    return to_string(0);
}

// DONE - TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
    
    string result;
    string line = getContentOfLine<string>(to_string(pid)+kStatFilename);
    istringstream linestream(line);

    // Catch execption in case not all data is present in file
    try {
        for (int i=0; i<22; i++) {
            linestream >> result;
        }
    }
    catch (...) {
        result = to_string(0);
    }
    
    long upTimePid = UpTime() - mystol(result) / HERTZ;
    return upTimePid;
 }