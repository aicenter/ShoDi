/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

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
