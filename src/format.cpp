#include <string>
#include <iostream>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    long hours = seconds/3600;
    long restSeconds = seconds%3600;
    long minutes = restSeconds/60;
    long secs = restSeconds%60;
    string strHour = to_string(hours);
    string strMinutes = to_string(minutes);
    string strSecs = to_string(secs);
    if(strMinutes.size() < 2){
        strMinutes = "0"+strMinutes;
    }
    if(strSecs.size() < 2){
        strSecs = "0"+strSecs;
    }

    
    return strHour+":"+strMinutes+":"+strSecs+"\n";
}