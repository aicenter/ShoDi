//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include "Timer.h"

//______________________________________________________________________________________________________________________
Timer::Timer(const string x) : realTimeSum(0), name(x) {
}

//______________________________________________________________________________________________________________________
void Timer::begin() {
    realBeginTimePoint = chrono::steady_clock::now();
}

//______________________________________________________________________________________________________________________
void Timer::finish() {
    realFinishTimePoint = chrono::steady_clock::now();

    realTimeSum += std::chrono::duration_cast<std::chrono::microseconds>(realFinishTimePoint - realBeginTimePoint);
}

//______________________________________________________________________________________________________________________
void Timer::reset() {
    realTimeSum = std::chrono::milliseconds(0);
}

//______________________________________________________________________________________________________________________
void Timer::printMeasuredTime() {
    cout << "Timer '" << name << "': Real time: " << getRealTimeSeconds() << endl;
}

//______________________________________________________________________________________________________________________
double Timer::getRealTimeSeconds() {
    return ((double) realTimeSum.count()) / 1000000.0;
}
