#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

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
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
    string line, variable;
    float memtotal, memfree, usedmemory;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open())
    {
      std::getline(stream, line);
      std::istringstream iss1(line);
      iss1>> variable >> memtotal;
      std::getline(stream, line);
      std::istringstream iss2(line);
      iss2>> variable >> memfree;
      usedmemory= (memtotal - memfree) /memtotal;
      return usedmemory;
    }
    else{
      return 0.0;
    }

}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptimesec;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);

    linestream >> uptimesec;
  }
  return uptimesec; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long all_jiffies = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  return all_jiffies;

 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line, variable;
  int counter = 13;
  long utime, stime, cutime, cstime;
  if(stream.is_open()){
      std::getline(stream, line);
      std::istringstream iss(line);
      for(int i = 0; i<counter; ++i)
      {
        iss>>variable;
      }

      iss >> utime >> stime >> cutime >> cstime;

      return utime+stime+cutime+cstime;
  }
  else
  {
    return 0;
  }
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    vector<string> jiffies = LinuxParser::CpuUtilization();
    long user = std::stol(jiffies[0]);
    long nice = std::stol(jiffies[1]);
    long system = std::stol(jiffies[2]);
    long irq = std::stol(jiffies[5]);
    long softirq = std::stol(jiffies[6]);
    long steal = std::stol(jiffies[7]);

    long idle = user + nice + system + irq + softirq + steal;

    return idle;
    }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    vector<string> jiffies = LinuxParser::CpuUtilization();
    long idle = std::stol(jiffies[3]);
    long iowait = std::stol(jiffies[4]);


    long non_idle = idle + iowait;

    return non_idle;

}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  string line;
  string cpu_name, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream iss(line);
    iss >>cpu_name>> user>> nice>> system>> idle>> iowait>> irq>> softirq>> steal>> guest>> guest_nice;
  }
  return vector<string>{user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice}; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
 { 
    string line, variable;
    int totalprocesses;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open())
    {
      while(std::getline(stream, line))
      {
        std::istringstream iss(line);
        iss>>variable;
        if("processes" == variable)
        {
          iss>>totalprocesses;
          return totalprocesses;
        }
      }
    }
    else{
        return 0;
    }
  
    return 0; 
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{
    string line, variable;
    int runningprocesses;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open())
    {
      while(std::getline(stream, line))
      {
        std::istringstream iss(line);
        iss>>variable;
        if("procs_running" == variable)
        {
          iss>>runningprocesses;
          return runningprocesses;
        }

      }
    }
    else{
      return 0;
    }

    return 0;    
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line, command;
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream iss(line);
    iss >>command;
    return command;

  }
  else{
    return "";
  }

}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{   
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, variable;
  long ram;
  if (stream.is_open())
  {
    while(std::getline(stream, line))
    {
        std::istringstream iss(line);
        iss>>variable;
        if("VmSize:" == variable)
        {
          iss>>ram;
          return to_string(ram/1000);
        }

    }
  }
  else
  {
    return "";
  }
  return ""; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
    string line, variable;
    string uid;
    if(stream.is_open())
    {
      while(std::getline(stream, line))
      {
        std::istringstream iss(line);
        iss >> variable;
        if("Uid:" == variable)
        {
          iss>>uid;
          return uid;
        }
      }
    
    }

    return "";

}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string uid = LinuxParser::Uid(pid);
    std::ifstream stream(kPasswordPath);
    string line, variable1, variable2, user;
    while(std::getline(stream, line)){
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream iss(line);
        iss>>user;
        /* get uid field */
        iss>>variable1>>variable2;

        if(uid == variable2)

            return user;
    }
    return ""; 
  }


// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
 { 
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    string line, variable;
    long process_start_time;

    int counter = 23;
    if(stream.is_open()){
        std::getline(stream, line);
        std::istringstream iss(line);
        for(int i = 0; i<counter; ++i)
        {
          iss>>variable;
        }

      /*  while(--counter){
            iss>>variable;
        }*/
        iss>>process_start_time;
    }

    return LinuxParser::UpTime()-(process_start_time/sysconf(_SC_CLK_TCK));
 }