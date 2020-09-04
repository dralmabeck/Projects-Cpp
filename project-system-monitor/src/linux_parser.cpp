#include "linux_parser.h"

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
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
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
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

// DONE - TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

    float total, free, buffer, cache;
    string line, key, result;
    ifstream stream(kProcDirectory + kMeminfoFilename);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            istringstream linestream(line);
            while (linestream >> key >> result) {
                if (key == "MemTotal:" ) {
                    total = stof(result);
                }
                if (key == "MemFree:" ) {
                    free = stof(result);
                }
                if (key == "Buffers:" ) {
                     buffer = stof(result);
                }
                if (key == "Cached:" ) {
                    cache = stof(result);
                }
            }
        }
    }
  
    return (1.0 - (free + buffer + cache) / total);
 }

// DONE - TODO: Read and return the system uptime
long LinuxParser::UpTime() { 

    string line, result;
    ifstream stream(kProcDirectory + kUptimeFilename);

    if (stream.is_open()) {
        getline(stream, line);
        istringstream linestream(line);
        if (linestream >> result) {
            return stol(result);
        }
    }

    return 0;
}

// DONE - TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 

    vector<string> jiffies;
    string line, key, result;
    ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

    if (stream.is_open()) {
        getline(stream, line);
        istringstream linestream(line);
        while (linestream >> key) {
            jiffies.push_back(key);
        }
        long result = stol(jiffies[13]) + stol(jiffies[14]) + stol(jiffies[15]) + stol(jiffies[16]);
        return result;
    }

    return 0;
 }

// DONE - TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 

    string line, key;
    vector<string> result;
    ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {
        getline(stream, line);
        istringstream linestream(line);
        while (linestream >> key) {
            if (key != "cpu") {
                result.push_back(key);
            }
        }
    }

  return result;
}

// DONE - TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 

    string line, key, result;
    ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            istringstream linestream(line);
            while (linestream >> key >> result) {
                if (key == "processes" ) {
                    return stoi(result);
                }
            }
        }
    }

  return 0;
}

// DONE - TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {

    string line, key, result;
    ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {
      while (getline(stream, line)) {
        istringstream linestream(line);
        while (linestream >> key >> result) {
          if (key == "procs_running" ) {
            return stoi(result);
          }
        }
      }
    }

    return 0;
}

// DONE - TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 

    string line;
    ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);

    if (stream.is_open()) {
        getline(stream, line);
        return line;
    }

    return to_string(0);
}

// DONE - TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {

    string key, result;
    ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

    if (stream.is_open()) {
      while (stream >> key) {
        if (key == "VmSize:" ) {
          if (stream >> result) {
            result = to_string(stol(result) / 1024);
            return result;
          }
        }
      }
    }

    return to_string(0);
}

// DONE - TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 

    string line, key, result;
    ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

    if (stream.is_open()) {
        getline(stream, line);
        istringstream linestream(line);
        while (linestream >> key >> result) {
            if (key == "Uid:" ) {
                return result;
            }
        }
    }

    return to_string(0);
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
    
    string a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, result;
    ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

    if (stream.is_open()) {
        stream >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7 >> a8 >> a9 >> a10 >> a11 >> a12 >> a13 >> result;
        return stol(result) / sysconf(_SC_CLK_TCK);
    }

    return 0;
 }