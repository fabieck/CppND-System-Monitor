#include "processor.h"
#include "linux_parser.h"
#include <iostream>
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    float active_jiffies = LinuxParser::ActiveJiffies();
    float all_jiffies = LinuxParser::Jiffies();

    float cpu_utilization =(active_jiffies-last_active_jiffies) / (all_jiffies-last_all_jiffies);

    last_active_jiffies = active_jiffies;

    last_all_jiffies = all_jiffies;


    return cpu_utilization; 
}