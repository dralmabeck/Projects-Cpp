#ifndef PROCESS_H
#define PROCESS_H

#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
    public:
        int Pid() const;                          // DONE - TODO: See src/process.cpp
        string User() const;                      // DONE - TODO: See src/process.cpp
        string Command() const;                   // DONE - TODO: See src/process.cpp
        double CpuUtilization() const;             // DONE - TODO: See src/process.cpp
        string Ram() const;                       // DONE - TODO: See src/process.cpp
        long int UpTime() const;                  // DONE - TODO: See src/process.cpp
        bool operator<(Process const& a) const;   // DONE - TODO: See src/process.cpp
        bool operator>(Process const& a) const;   // DONE - TODO: See src/process.cpp
        Process(int pid) : pid(pid) {};

    // DONE - TODO: Declare any necessary private members
    private:
       int pid;
};

#endif