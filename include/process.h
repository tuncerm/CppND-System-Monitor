#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
    Process(int pid, string user, string cmd, float cpu_u, string ram_u, int uptime);
    int Pid();                               // DONE: See src/process.cpp
    std::string User();                      // DONE: See src/process.cpp
    std::string Command();                   // DONE: See src/process.cpp
    float CpuUtilization();                  // TODO: See src/process.cpp
    std::string Ram();                       // DONE: See src/process.cpp
    std::string UpTime();                    // DONE: See src/process.cpp
    bool operator<(Process const &a) const;  // DONE: See src/process.cpp

    // DONE: Declare any necessary private members
private:
    int pid_;
    string user_;
    string command_;
    float cpu_;
    string ram_;
    int uptime_;
};

#endif