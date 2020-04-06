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
    int Pid();                               // TODO: See src/process.cpp
    std::string User();                      // TODO: See src/process.cpp
    std::string Command();                   // TODO: See src/process.cpp
    float CpuUtilization();                  // TODO: See src/process.cpp
    std::string Ram();                       // TODO: See src/process.cpp
    long int UpTime();                       // TODO: See src/process.cpp
    bool operator<(Process const &a) const;  // TODO: See src/process.cpp

    // TODO: Declare any necessary private members
private:
    int pid_, uptime_;
    string user_, command_, ram_;
    float cpu_;
};

#endif