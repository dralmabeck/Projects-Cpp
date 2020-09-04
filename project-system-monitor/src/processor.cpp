#include "processor.h"

// DONE - TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    // Placeholder variables for all with space seperated content in /proc/stat file
    // See LinuxParser::CPUStates: kUser, kNice, kSystem, kIdle, kIOwait, kIRQ, kSoftIRQ, kSteal, kGuest, kGuestNice

    string line, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11;

    // Path to /proc/stat file
    ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

    // First row in file contains aggregated CPU information, parse and store in strings
    if (stream.is_open()) {
        getline(stream, line);
        istringstream linestream(line);
        linestream >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7 >> a8 >> a9 >> a10 >> a11;
    }

    // Calculate CPU Utilization according to http://stackoverflow.com/a/23376195
    long idle = stol(a5) + stol(a6);
    long active = stol(a2) + stol(a3) + stol(a4) + stol(a7) + stol(a8) + stol(a9);

    float total = (float)(idle + active);

    float result = (total - idle) / total;

    return result;
 }