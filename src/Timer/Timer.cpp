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

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include "Timer.h"

//______________________________________________________________________________________________________________________
Timer::Timer(const std::string x) : realTimeSum(0), name(x) {
}

//______________________________________________________________________________________________________________________
void Timer::begin() {
    realBeginTimePoint = std::chrono::steady_clock::now();
}

//______________________________________________________________________________________________________________________
void Timer::finish() {
    realFinishTimePoint = std::chrono::steady_clock::now();

    realTimeSum += std::chrono::duration_cast<std::chrono::microseconds>(realFinishTimePoint - realBeginTimePoint);
}

//______________________________________________________________________________________________________________________
void Timer::reset() {
    realTimeSum = std::chrono::milliseconds(0);
}

//______________________________________________________________________________________________________________________
void Timer::printMeasuredTime() {
    std::cout << "Timer '" << name << "': Real time: " << getRealTimeSeconds() << std::endl;
}

//______________________________________________________________________________________________________________________
double Timer::getRealTimeSeconds() {
    return ((double) realTimeSum.count()) / 1000000.0;
}
