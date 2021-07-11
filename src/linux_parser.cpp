#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include <unistd.h>
#include <filesystem>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T findValueByKeyInFile(std::string const &filename,std::string const &key, uint16_t position = 0){
  // Input parameters description:
  // filename: string with the name of the file to look into
  // key: string with the name of the requested parameter
  // position: integer representing how many strings away from the keyword is the needed value, on the same line
  //            0 means --> the next string

  std::ifstream filestream(filename);
  T value{};
  std::string line{},tempKey{},tempStr{};

  // main loop 
  if (filestream.is_open()){
    if (key.size()>0){
      while (std::getline(filestream,line)){
        std::istringstream linestream{line};
        while (linestream >> tempKey){
          if (tempKey == key){
            // the requested line where the value is was found
            // move away from the keyword position times
            for (uint16_t i=0;i<position+1;i++){
              linestream >> value;
            }
            return value;
          }
        }
      }
    } else {// key is empty --> this means that the file has only one line
      std::getline(filestream,line);
      std::istringstream linestream{line};
      // move along the line position times
      for (uint16_t i=0;i<position+1;i++){
        linestream >> value;
      }
      return value;
    }
  }
  return value;
}

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == kFilterOSName) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string kernel{findValueByKeyInFile<string>(kProcDirectory + kVersionFilename,"",kKernelPos)};
  return kernel;
}

vector<int> LinuxParser::Pids(){
  vector <int> pids{};
  for (auto& p: std::filesystem::directory_iterator(kProcDirectory)){
    if(p.is_directory()){
      if(p.path().stem().string().size()>0 && p.path().stem().string().substr(0,1) >= "0" && p.path().stem().string().substr(0,1) <= "9"){
        int pid = std::stol(p.path().stem().string());
        pids.push_back(pid);
      }
    }   
  }
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float MemTotal{findValueByKeyInFile<float>(kProcDirectory+kMeminfoFilename,kFilterMemTotal)};
  float MemFree{findValueByKeyInFile<float>(kProcDirectory+kMeminfoFilename,kFilterMemFree)};
  float Buffers{findValueByKeyInFile<float>(kProcDirectory+kMeminfoFilename,kFilterBuffers)};
  float memUtil = 1.0 - ((MemFree)/(MemTotal-Buffers));
  return memUtil; 
}

long LinuxParser::UpTime() { 
  long upTime{findValueByKeyInFile<long>(kProcDirectory+kUptimeFilename,"",kUpTimePos)};
  return upTime;
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> results{};
  for (uint16_t i=kUser_;i<=kGuestNice_;i++){
    results.push_back(findValueByKeyInFile<string>(kProcDirectory+kStatFilename,kFilterCpuUtil,i));
  }  
  return results;
}

int LinuxParser::TotalProcesses() {
  int processes{findValueByKeyInFile<int>(kProcDirectory+kStatFilename,kFilterProcesses)};
  return processes;
}

int LinuxParser::RunningProcesses() {
  int running_processes{findValueByKeyInFile<int>(kProcDirectory+kStatFilename,kFilterRunningProcesses)};
  return running_processes;
 }

string LinuxParser::Command(int pid) {
  string command{findValueByKeyInFile<string>(kProcDirectory+std::to_string(pid)+kCmdlineFilename,"")};
  return command;
}

string LinuxParser::Ram(int pid) {
  // the vmsize is used, according to project guidelines
  // alternatively, the VmRSS could be used
  long vmsize{findValueByKeyInFile<long>(kProcDirectory+std::to_string(pid)+kStatusFilename,kFilterRam)};
  return std::to_string(vmsize/kRamScale);
}

string LinuxParser::Uid(int pid) {
  string uid{findValueByKeyInFile<string>(kProcDirectory+std::to_string(pid)+kStatusFilename,kFilterUid)};
  return uid;
}

string LinuxParser::User(int uid) {
  std::ifstream stream(kPasswordPath);
  string line{""},username{""},uidStr{""},tempStr{""},tempChar{""};
  int counter{0};
  uint linepos{0};
  bool uidFound{false};
  if (stream.is_open()){
    while (std::getline(stream,line) && !uidFound){
      // process each line
      counter = 0;
      linepos = 0;
      tempStr = "";
      while (linepos < line.size() && counter < 3){
        tempChar = line.at(linepos);
        if( tempChar == ":"){
          if (counter == 0){
            username = tempStr;
            tempStr = "";
            counter++;
          } else if ( counter == 1){
            tempStr = "";
            counter++;
          } else if (counter == 2){
            uidStr = tempStr;
            counter++;
            if (uidStr.size()>0){
              if (std::stol(uidStr) == uid){ uidFound = true;}
            }
          }
        } else {
          tempStr += tempChar;
        }
        linepos++;
      }
    }
  }
  return username;
}

long LinuxParser::UpTime(int pid) {
  string strValue{};
  long upTimePid{};
  // get the Linux kernel version
  string kernelVersion = LinuxParser::Kernel();
  // extract the first two dot(.) separated integers
  string numb1{},numb2{};
  uint counter{0},strPos{0};
  while ( strPos < kernelVersion.size() && counter <=2){
    if (kernelVersion.at(strPos) != '.'){
      if (counter == 0){
        numb1+=kernelVersion.at(strPos);
      } else if (counter == 1){
        numb2+=kernelVersion.at(strPos);
      }
    } else { // char is a dot
      counter++;
    }
    strPos++;
  }
  string shortedVersion{numb1+"."+numb2};
  if( shortedVersion >= kOldLinuxVersion){
    // obtain time in clock ticks
    strValue = findValueByKeyInFile<string>(kProcDirectory+std::to_string(pid)+kStatFilename,"",kPidUptimePos);
    upTimePid = LinuxParser::UpTime()- std::stol(strValue)/sysconf(_SC_CLK_TCK);
  } else {
    // obtain time in jiffies
    // according to the source, the conversion is still done by diving with sysconf(_SC_CLK_TCK)
    // source: https://stackoverflow.com/questions/3875801/convert-jiffies-to-seconds
    strValue = findValueByKeyInFile<string>(kProcDirectory+std::to_string(pid)+kStatFilename,"",kPidUptimePos);
    upTimePid = LinuxParser::UpTime()- std::stol(strValue)/sysconf(_SC_CLK_TCK);
  }  
  return upTimePid;
}

std::vector<long> LinuxParser::CpuUtilData(int pid){
  std::vector<long> data{};
  for(const uint16_t &i: kCpuUtilPos){
    std::string tempStr{findValueByKeyInFile<string>(kProcDirectory+std::to_string(pid)+kStatFilename,"",i)};
    if (tempStr.size()>0){
      data.push_back(std::stol(tempStr));
    } else {
      data.push_back(0.0);
    }
  }
  return data;
}