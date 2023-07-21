//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_TIMER_H
#define TRANSIT_NODE_ROUTING_TIMER_H

#include <chrono>
#include <ctime>
#include <string>



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
    Timer(const std::string x);

    /**
     * Function to start the timer.
     */
    void begin();

    /**
     * Function to end the timer. The time between the call of 'begin' and the call of 'finish' will be measured.
     */
    void finish();

    /**
     * Function to reset the timer to 0.
     */
    void reset();

    /**
     * Prints the measured time as a std::string in seconds.
     */
    void printMeasuredTime();

    /**
     * Returns the measured real time in seconds. The measured time is the real time between each call to the 'begin' function
     * and to the 'finish' function. Both 'begin' and 'finish' can be called multiple times on the same timer
     * and the sum will be returned, unless 'reset' is called.
     *
     * @return The real time measured by the timer in seconds.
     */
    double getRealTimeSeconds();

private:
    std::chrono::microseconds realTimeSum;
    std::chrono::steady_clock::time_point realBeginTimePoint;
    std::chrono::steady_clock::time_point realFinishTimePoint;
    const std::string name;
};


#endif //TRANSIT_NODE_ROUTING_TIMER_H
