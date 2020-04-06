#include "processor.h"
#include "linux_parser.h"

using namespace std;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    long idlejiffies = LinuxParser::IdleJiffies();
    long activeJiffies = LinuxParser::ActiveJiffies();
    long total = idlejiffies + activeJiffies;
    float difftotal = total - last_total;
    float diffidle = idlejiffies - last_idle;

    last_total = total;
    last_idle = idlejiffies;

    float cpu_util = (difftotal - diffidle) / difftotal;

    return cpu_util;
}