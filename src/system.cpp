#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <fstream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// DONE: Return the system's CPU
Processor &System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process> &System::Processes() {
    vector<Process> nprocs = processes_;

    processes_ = {};

    vector<int> pids = LinuxParser::Pids();

    for(Process process:nprocs){
        int p_id = process.Pid();
        for(unsigned int i = 0; i < pids.size(); i++){
            if(pids[i]==p_id){
                process.Update();
                processes_.push_back(process);
                pids.erase(pids.begin()+i--);
            }
        }
    }

    for (int pid:pids) {
        Process process(pid, LinuxParser::User(pid), LinuxParser::Command(pid), LinuxParser::ActiveJiffies(pid),
                        LinuxParser::Ram(pid), LinuxParser::UpTime(pid));
        processes_.push_back(process);
    }

    std::sort(processes_.begin(), processes_.end(), [](Process a, Process b) { return a < b; });
    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }