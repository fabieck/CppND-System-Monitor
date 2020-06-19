#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

#include "process.h"

using std::string;
using std::to_string;
using std::vector;
Process::Process(int pid) : _pid(pid)
{
    _cpuUtilization = CpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{
    return (LinuxParser::ActiveJiffies(_pid)/sysconf(_SC_CLK_TCK))/(float)LinuxParser::UpTime(_pid); 
}

// TODO: Return the command that generated this process
string Process::Command()
{
    string command = LinuxParser::Command(_pid);
    return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() 
{
    string ram = LinuxParser::Ram(_pid);
    return ram; 
}

// TODO: Return the user (name) that generated this process
string Process::User()
{
    std::string user = LinuxParser::User(_pid);
    return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{
    long int uptime = LinuxParser::UpTime(_pid);
    return uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{
    return  _cpuUtilization<a._cpuUtilization;    
}