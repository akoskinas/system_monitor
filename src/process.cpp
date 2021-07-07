#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include <unistd.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
    pid_ = pid;
    uid_ = LinuxParser::Uid(pid_);
    if (uid_.size()>0){
        user_ = LinuxParser::User(std::stoi(uid_));
        command_ = LinuxParser::Command(pid_);
        cpuUtilization_ = Process::CpuUtilization();
        ram_ = LinuxParser::Ram(pid_);
        upTime_ = LinuxParser::UpTime(pid_);
    }   
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    float cpuUtil{0.0};
    std::vector<long> data = LinuxParser::CpuUtilData(pid_);
    long utime{data.at(0)}, stime{data.at(1)},cutime{data.at(2)},cstime{data.at(3)},starttime{data.at(4)};
    
    // processing 
    long total_time = utime + stime +cutime + cstime;
    long seconds = LinuxParser::UpTime() - (starttime/sysconf(_SC_CLK_TCK));
    if (seconds > 0){
        cpuUtil = ((float)total_time/sysconf(_SC_CLK_TCK))/seconds;
    }
    return cpuUtil;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    bool flag{false};
    if (this->cpuUtilization_ > a.cpuUtilization_) {
        flag = true;
    } else { flag = false;}
    return flag;
}