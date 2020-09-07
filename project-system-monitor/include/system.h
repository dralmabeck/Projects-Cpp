#ifndef SYSTEM_H
#define SYSTEM_H

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System {
    public:
        Processor& Cpu();                   // DONE - TODO: See src/system.cpp
        vector<Process>& Processes();       // DONE - TODO: See src/system.cpp
        double MemoryUtilization();          // DONE - TODO: See src/system.cpp
        long UpTime();                      // DONE - TODO: See src/system.cpp
        int TotalProcesses();               // DONE - TODO: See src/system.cpp
        int RunningProcesses();             // DONE - TODO: See src/system.cpp
        string Kernel();                    // DONE - TODO: See src/system.cpp
        string OperatingSystem();           // DONE - TODO: See src/system.cpp

    // DONE - TODO: Define any necessary private members
    private:
        Processor cpu_ = {};
        vector<Process> processes_ = {};
};

#endif