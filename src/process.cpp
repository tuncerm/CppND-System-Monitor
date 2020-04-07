#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, string user, string cmd, long jiffies, string ram_u, long uptime) : pid_(pid), user_(user),
                                                                                              command_(cmd),
                                                                                              jiffies_(jiffies),
                                                                                              ram_(ram_u),
                                                                                              uptime_(uptime) {};

// Added Self Update Method.
void Process::Update() {
    jiffies_ = LinuxParser::ActiveJiffies(pid_);
    ram_ = LinuxParser::Ram(pid_);
    uptime_ = LinuxParser::UpTime(pid_);
    long dt = (uptime_ - last_uptime);
    cpu_ = dt == 0 ? 0 : (jiffies_ - last_jifjies) / dt;
    last_uptime = uptime_;
    last_jifjies = jiffies_;
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_ / uptime_; }
// float Process::CpuUtilization() { return cpu_ / (uptime_ * sysconf(_SC_CLK_TCK)); }

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return ram_; }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const { return cpu_ > a.cpu_; }