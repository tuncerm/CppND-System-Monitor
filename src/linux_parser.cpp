#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "PRETTY_NAME") {
                    std::replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }
        }
    }
    return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
    string os, v, kernel;
    string line;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> v >> kernel;
    }
    return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
    vector<int> pids;
    DIR *directory = opendir(kProcDirectory.c_str());
    struct dirent *file;
    while ((file = readdir(directory)) != nullptr) {
        // Is this a directory?
        if (file->d_type == DT_DIR) {
            // Is every character of the name a digit?
            string filename(file->d_name);
            if (std::all_of(filename.begin(), filename.end(), isdigit)) {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    float memory_free = 0, memory_total = 0;
    string line, key, value;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "MemTotal") {
                    memory_total = stof(value);
                    break;
                }
                if (key == "MemAvailable") {
                    memory_free = stof(value);
                    break;
                }
            }
            if (memory_total > 0 && memory_free > 0) {
                break;
            }
        }
    }
    return (memory_total - memory_free) / memory_total;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
    long uptime = 0;
    string line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> uptime;
    }
    return uptime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    return ActiveJiffies() + IdleJiffies();
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    long jiffies = 0;
    string line, value;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if(stream.is_open()){
        std::getline(stream, line);
        std::istringstream linestream(line);
        int i = 0;
        for(; i < 14; ++i){
            linestream >> value;
        }
        for(; i < 18; ++i){
            jiffies += std::stol(value);
        }
    }
    return jiffies;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    vector<string> cpustate = CpuUtilization();
    long jiffies = -1;
    if (cpustate.size() == 10) {
        jiffies = stol(cpustate[CPUStates::kUser_]) + stol(cpustate[CPUStates::kNice_]) +
                  stol(cpustate[CPUStates::kSystem_]) + stol(cpustate[CPUStates::kSteal_]) +
                  stol(cpustate[CPUStates::kIRQ_]) + stol(cpustate[CPUStates::kSoftIRQ_]);
    }
    return jiffies;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    vector<string> cpustate = CpuUtilization();
    long jiffies = -1;
    if (cpustate.size() == 10) {
        jiffies = stol(cpustate[CPUStates::kIdle_]) + stol(cpustate[CPUStates::kIOwait_]);
    }
    return jiffies;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    string line, value;
    vector<string> values;

    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> value;
        if (value == "cpu") {
            while (linestream >> value) {
                values.push_back(value);
            }
        }
    }

    return values;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string line, key, value;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "processes") {
                return std::stoi(value);
            }
        }
    }
    return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line, key, value;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "procs_running") {
                return std::stoi(value);
            }
        }
    }
    return 0;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string cmd, line;
    std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if(stream.is_open()){
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream>>cmd;
    }
    return cmd;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    string key, value, line;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
    if(stream.is_open()){
        while(std::getline(stream, line)){
            std::istringstream linestream(line);
            linestream >> key >> value;
            if(key == "VmSize:"){
                return value;
            }
        }
    }
    return "";
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string key, value, line;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
    if(stream.is_open()){
        while(std::getline(stream, line)){
            std::istringstream linestream(line);
            linestream >> key >> value;
            if(key == "Uid:"){
                return value;
            }
        }
    }
    return "";
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string user, line, key, value;
    std::ifstream stream(kPasswordPath);
    if(stream.is_open()){
        while(std::getline(stream, line)){
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> user >> key >> value;
            if(std::stoi(value) == pid){
                return user;
            }
        }
    }
    return "";
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    long uptime = -1;
    string line, value;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if(stream.is_open()){
        std::getline(stream, line);
        std::istringstream linestream(line);
        for(int i = 0; i < 23; ++i){
            linestream >> value;
        }
        uptime = UpTime() - std::stol(value)/sysconf(_SC_CLK_TCK);
    }
    return uptime;
}