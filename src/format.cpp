#include <string>
#include <sstream>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hours = seconds / 3600;
    int minutes = (seconds / 3600) % 60;
    seconds %= 60;
    std::ostringstream stream;
    stream << (hours > 9 ? "" : "0") << hours << ":";
    stream << (minutes > 9 ? "" : "0") << minutes << ":";
    stream << (seconds > 9 ? "" : "0") << seconds;
    return stream.str();
}