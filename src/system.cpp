#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() {
    cpu_.SetPreviousState(cpu_.GetNextState());
    cpu_.SetNextState(cpu_.GetTransitionState());
    cpu_.SetTransitionState(LinuxParser::CpuUtilization());
    return cpu_; 
}

vector<Process>& System::Processes() {
    vector<int> pids{LinuxParser::Pids()};
    // clear the vector. Alternative is to search the vector for the PID, and if it exists, then call updaters
    processes_.clear();

    for (int pid:pids){
        processes_.emplace_back(pid);
    }
    // perform sorting
    std::sort(processes_.begin(),processes_.end());
    return processes_;
}

std::string System::Kernel() {
    kernel_ = LinuxParser::Kernel();
    return kernel_;
}

float System::MemoryUtilization() {
    memoryUtilization_ = LinuxParser::MemoryUtilization();
    return memoryUtilization_;
}

std::string System::OperatingSystem() {
    operatingSystem_ = LinuxParser::OperatingSystem();
    return operatingSystem_;
}

int System::RunningProcesses() {
    runningProcesses_ = LinuxParser::RunningProcesses();
    return runningProcesses_;
}

int System::TotalProcesses() {
    totalProcesses_ = LinuxParser::TotalProcesses();
    return totalProcesses_;
}

long int System::UpTime() { 
    upTime_ = LinuxParser::UpTime();
    return upTime_;
}