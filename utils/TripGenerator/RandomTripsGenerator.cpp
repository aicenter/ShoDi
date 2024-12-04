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

#include <cstdio>
#include <cstdlib>

#define NODES_CNT 23947347 //23947347
#define TRIPS_AMOUNT 1000

// Auxiliary program for generating random trips with source and target nodes in a certain range.
// Using rand() isn't generally the best way to generate truly random numbers, but it's random enough for our needs.
//______________________________________________________________________________________________________________________
int main() {
    printf("%u\n", TRIPS_AMOUNT);
    for (unsigned int i = 0; i < TRIPS_AMOUNT; i++) {
        unsigned int source = (rand() % static_cast<int>(NODES_CNT));
        unsigned int target = (rand() % static_cast<int>(NODES_CNT));
        printf("%u %u\n", source, target);
    }

    return 0;
}