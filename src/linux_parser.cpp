#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include <unistd.h>
#include <filesystem>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
//vector<int> LinuxParser::Pids() {
//  vector<int> pids;
//  DIR* directory = opendir(kProcDirectory.c_str());
//  struct dirent* file;
//  while ((file = readdir(directory)) != nullptr) {
//    // Is this a directory?
//    if (file->d_type == DT_DIR) {
//      // Is every character of the name a digit?
//      string filename(file->d_name);
//      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
//        if(filename.size()>0){
//          int pid = std::stol(filename);
//          pids.push_back(pid);
//        }        
//      }
//    }
//  }
//  closedir(directory);
//  return pids;
//}

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

// TODO: Read and return the system memory utilization // UPD: Done
float LinuxParser::MemoryUtilization() {
  long double MemTotal{0}, MemFree{0}, MemAvailable{0}, Buffers{0};
  string line{""},label{""};
  std::istringstream temp{""};
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  if (stream.is_open()){
    for (int i = 0; i < 4; i++){
      // extract the first value:
      std::getline(stream,line);
      temp.str(line);
      temp >> label;
      if (label == "MemTotal:"){
        temp >> MemTotal;
      } else if ( label == "MemFree:"){
        temp >> MemFree;
      } else if (label == "MemAvailable:"){
        temp >> MemAvailable;
      } else if  (label == "Buffers:"){
        temp >> Buffers;
      }
    }   
  }
  float memUtil = 1.0 - ((MemFree)/(MemTotal-Buffers));
  return memUtil; 
}

// TODO: Read and return the system uptime // UPD: Done
long LinuxParser::UpTime() { 
  long upTime{0};
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  string line;
  std::istringstream temp{""};
  if (stream.is_open()) {
    std::getline(stream,line);
    temp.str(line);
    temp >> upTime;
    //std::cout << "upTime: " << upTime << std::endl;
  }
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {return 0;}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory+kStatFilename);
  string line{},label{},tempStr{};
  std::istringstream temp{};
  vector<string> results{};
  if (stream.is_open()){
    while (std::getline(stream,line)){
      temp.str(line);
      if (temp >> label) {
        if (label == "cpu"){
          for (int i=kUser_;i<=kGuestNice_;i++){
            temp>>tempStr;
            results.push_back(tempStr);             
          }
        }         
      }
    }
  }
  return results;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory+kStatFilename);
  string line,firstString;
  int processes(0);
  std::istringstream temp("");
  if (stream.is_open()){
    while (std::getline(stream,line)){
      temp.str(line);
      if (temp >> firstString){
        if (firstString == "processes"){
          temp >> processes;
          break;
        }
      } 
    }
  }
  return processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory+kStatFilename);
  string line,firstString;
  int running_processes(0);
  std::istringstream temp("");
  if (stream.is_open()){
    while (std::getline(stream,line)){
      temp.str(line);
      if (temp >> firstString){
        if (firstString == "procs_running"){
          temp >> running_processes;
          break;
        }
      } 
    }
  }
  return running_processes;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  string command{""};
  if (stream.is_open()){
    std::getline(stream,command);
  } 
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  string line{},label{};
  long vmsize;
  std::istringstream temp("");
  if (stream.is_open()){
    while (std::getline(stream,line)){
      temp.str(line);
      if (temp >> label) {
        if (label == "VmSize:"){
          if (temp >> vmsize){ break;}
        }
      }      
    }    
  }
  return std::to_string(vmsize/1000);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  string line{""},label{""},uid{""};
  std::istringstream temp{};
  if (stream.is_open()){
    while(std::getline(stream,line)){
      temp.str(line);
      if (temp >> label){
        if (label == "Uid:"){ temp >> uid; }
      }      
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
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

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatFilename);
  string temp;
  long int upTime;
  int counter{0};
  if (stream.is_open()){
    while(stream >> temp){
      counter++;
      if (counter == 22){
        if (temp.size()>0){
          upTime = (std::stol(temp)/sysconf(_SC_CLK_TCK));
        }       
      }
    }
  }
  return upTime;
}

std::vector<long> LinuxParser::CpuUtilData(int pid){
  std::vector<long> data {};
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatFilename);
  std::string temp;
  int counter{0};
  if (stream.is_open()){
    while (stream >> temp){
      counter++;
      if (counter == 14 || counter == 15 || counter == 16 || counter == 17 || counter == 22){
        if (temp.size()>0){
          data.push_back(std::stol(temp));
        } else {
          data.push_back(0.0);
        }
      }      
    }    
  }
  return data;
}

