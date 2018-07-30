//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#include <cstdio>
#include <string>
#include "Timer.h"

//______________________________________________________________________________________________________________________
Timer::Timer(const string x) : name(x) {
}

//______________________________________________________________________________________________________________________
void Timer::begin() {
    beginTimePoint = chrono::steady_clock::now();
}

//______________________________________________________________________________________________________________________
void Timer::finish() {
    finishTimePoint = chrono::steady_clock::now();
}

//______________________________________________________________________________________________________________________
void Timer::printMeasuredTime() {
    printf("Timer '%s' counted %lf seconds.\n", name.c_str(), (std::chrono::duration_cast<std::chrono::microseconds>(finishTimePoint - beginTimePoint).count()) /1000000.0);
}

//______________________________________________________________________________________________________________________
double Timer::getMeasuredTimeInSeconds() {
    return (std::chrono::duration_cast<std::chrono::microseconds>(finishTimePoint - beginTimePoint).count()) /1000000.0;
}