#include "processor.h"

// DONE - TODO: Return the aggregate CPU utilization
double Processor::Utilization() { 
    
    // Placeholder variables for all with space seperated content in /proc/stat file
    // See LinuxParser::CPUStates: kUser, kNice, kSystem, kIdle, kIOwait, kIRQ, kSoftIRQ, kSteal, kGuest, kGuestNice

    string line, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11;
    double result;

    // Path to /proc/stat file
    ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

    // First row in file contains aggregated CPU information, parse and store in strings
    try {
        if (stream.is_open()) {
            getline(stream, line);
            istringstream linestream(line);
            linestream >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7 >> a8 >> a9 >> a10 >> a11;
        }

        // Calculate CPU Utilization according to http://stackoverflow.com/a/23376195
        long idle = mystol(a5) + mystol(a6);
        long active = mystol(a2) + mystol(a3) + mystol(a4) + mystol(a7) + mystol(a8) + mystol(a9);

        double total = static_cast<double>(idle + active);

        result = (total - idle) / total;
    }
    catch (...) {
        result = 0.0;
    }

    return result;
 }