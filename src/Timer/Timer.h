//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_TIMER_H
#define TRANSIT_NODE_ROUTING_TIMER_H

#include <chrono>

using namespace std;

class Timer {
public:
    Timer(const string x);
    void begin();
    void finish();
    void printMeasuredTime();
    double getMeasuredTimeInSeconds();
private:
    chrono::steady_clock::time_point beginTimePoint;
    chrono::steady_clock::time_point finishTimePoint;
    const string name;
};


#endif //TRANSIT_NODE_ROUTING_TIMER_H
