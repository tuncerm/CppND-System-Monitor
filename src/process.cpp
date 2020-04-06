#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "format.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, string user, string cmd, float cpu_u, string ram_u, int uptime) : pid_(pid), user_(user),
                                                                                            command_(cmd), cpu_(cpu_u),
                                                                                            ram_(ram_u),
                                                                                            uptime_(uptime) {};

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
string Process::UpTime() { return Format::ElapsedTime(uptime_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const { return cpu_ < a.cpu_; }