#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

// include the namespace
#include "linux_parser.h"
// include the iostream
#include <iostream>

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() {
    cpu_.SetPreviousState(cpu_.GetNextState());
    cpu_.SetNextState(cpu_.GetTransitionState());
    cpu_.SetTransitionState(LinuxParser::CpuUtilization());
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int> pids{LinuxParser::Pids()};
    // clear the vector // alternative is to search the vector for the PID, and if it exists, then call updaters
    processes_.clear();

    for (int pid:pids){
        processes_.push_back(Process(pid));
    }
    // needs to sort
    std::sort(processes_.begin(),processes_.end());
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
    kernel_ = LinuxParser::Kernel();
    return kernel_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
    memoryUtilization_ = LinuxParser::MemoryUtilization();
    return memoryUtilization_;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
    operatingSystem_ = LinuxParser::OperatingSystem();
    return operatingSystem_;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
    runningProcesses_ = LinuxParser::RunningProcesses();
    return runningProcesses_;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
    totalProcesses_ = LinuxParser::TotalProcesses();
    return totalProcesses_;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    upTime_ = LinuxParser::UpTime();
    return upTime_;
}