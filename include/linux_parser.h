#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <vector>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// Filter words
const std::string kFilterMemTotal{"MemTotal:"};
const std::string kFilterMemFree{"MemFree:"};
const std::string kFilterBuffers{"Buffers:"};
const std::string kFilterOSName{"PRETTY_NAME"};
const std::string kFilterCpuUtil{"cpu"};
const std::string kFilterProcesses{"processes"};
const std::string kFilterRunningProcesses{"procs_running"};
const std::string kFilterRam{"VmSize:"};
const std::string kFilterUid{"Uid:"};
const std::string kOldLinuxVersion{"2.6"};

// Positions inside the line after keywords
const uint16_t kKernelPos{2};
const uint16_t kUpTimePos{0};
const u_int16_t kRamScale{1000};
const u_int16_t kPidUptimePos{21};
const std::vector<u_int16_t> kCpuUtilPos{13,14,15,16,21};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();

int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

std::vector<std::string> CpuUtilization();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
std::vector<long> CpuUtilData(int pid);
};  // namespace LinuxParser

#endif