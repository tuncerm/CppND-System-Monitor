#include <string>

#include "processor.h"
#include "linux_parser.h"

using namespace std;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    string line, u_cpu, u_user, u_nice, u_system, u_idle, u_iowait, u_irq, u_softirq, u_steal, u_guest, u_guest_nice;
    float cpu_util = 0;
    ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            istringstream linestream(line);
            while (linestream >> u_cpu >> u_user >> u_nice >> u_system >> u_idle >> u_iowait >> u_irq >> u_softirq
                              >> u_steal >> u_guest >> u_guest_nice) {
                if (u_cpu == "cpu") {
                    int total =
                            stoi(u_user) + stoi(u_nice) + stoi(u_system) + stoi(u_idle) + stoi(u_iowait) + stoi(u_irq) +
                            stoi(u_softirq) + stoi(u_steal);
                    cpu_util = (total - last_total - stoi(u_idle) + last_idle) / (total - last_total);
                    last_total = total;
                    last_idle = stoi(u_idle);
                    break;
                }
            }
        }
    }

    return cpu_util;
}