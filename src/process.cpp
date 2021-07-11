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
    // if statement to avoid crushes in case of error in reading uid_
    if (uid_.size()>0){
        user_ = LinuxParser::User(std::stoi(uid_));
        command_ = LinuxParser::Command(pid_);
        cpuUtilization_ = Process::CpuUtilization();
        ram_ = LinuxParser::Ram(pid_);
        upTime_ = LinuxParser::UpTime(pid_);
    }
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
    float cpuUtil{0.0};
    std::vector<long> data = LinuxParser::CpuUtilData(pid_);
    long utime{data.at(0)}, stime{data.at(1)},cutime{data.at(2)},cstime{data.at(3)},starttime{data.at(4)};
    
    // processing 
    long total_time = utime + stime +cutime + cstime;
    long seconds = LinuxParser::UpTime() - (starttime/sysconf(_SC_CLK_TCK));
    if (seconds > 0){
        cpuUtil = (static_cast<float>(total_time)/sysconf(_SC_CLK_TCK))/seconds;
    }
    return cpuUtil;
}

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return upTime_; }

bool Process::operator<(Process const& a) const {
    return (this->cpuUtilization_ > a.cpuUtilization_);
}