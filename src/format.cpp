#include <string>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    string H{},M{},S{},total{};
    int hrs = (seconds/3600);
    int remainingTime = seconds - hrs * 3600;
    int mins = (remainingTime/60);
    remainingTime = remainingTime - mins*60;
    int secs = remainingTime;
    if (hrs <= 9){
        H = "0"+ std::to_string(hrs);
    } else {
        H = std::to_string(hrs);
    }
    if (mins <=9){
        M = "0"+std::to_string(mins);
    } else {
        M = std::to_string(mins);
    }
    if (secs <=9){
        S = "0"+std::to_string(secs);
    } else {
        S = std::to_string(secs);
    }
    total = H + ":" + M + ":" +S;
    return total;
    }