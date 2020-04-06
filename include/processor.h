#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
    float Utilization();  // DONE: See src/processor.cpp

    // DONE: Declare any necessary private members
private:
    int last_total = 0;
    int last_idle = 0;
};

#endif