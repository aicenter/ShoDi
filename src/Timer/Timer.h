//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_TIMER_H
#define TRANSIT_NODE_ROUTING_TIMER_H

#include <chrono>
#include <string>

using namespace std;

/**
 * Auxiliary class used for measurement. Each instance serves as a separate timer.
 */
class Timer {
public:
    /**
     * Initializes a timer.
     *
     * @param x[in] A optional name for the timer. This name will be used in output, so it can help with clarity.
     */
    Timer(const string x);

    /**
     * Function to start the timer.
     */
    void begin();

    /**
     * Function to end the timer. The time between the call of 'begin' and the call of 'finish' will be measured.
     */
    void finish();

    /**
     * Prints the measured time as a string in seconds.
     */
    void printMeasuredTime();

    /**
     * Returns the measured time in seconds. The measured time is the time between the call of the 'begin' function
     * and the call of the 'finish' function. Both 'begin' and 'finish' can be called multiple times on the same timer
     * and the returned times will still be correct.
     *
     * @return The time measured by the timer in seconds.
     */
    double getMeasuredTimeInSeconds();
private:
    chrono::steady_clock::time_point beginTimePoint;
    chrono::steady_clock::time_point finishTimePoint;
    const string name;
};


#endif //TRANSIT_NODE_ROUTING_TIMER_H
