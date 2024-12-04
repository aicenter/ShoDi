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
// Created on: 27.8.18
//

#define NODES_CNT 23947347 //23947347
#define TRIPS_AMOUNT 1000

#include <cmath>
#include <cstdlib>
#include <cstdio>

//______________________________________________________________________________________________________________________
inline unsigned int randomNodeID(unsigned int n) {
    return (unsigned int)(rand() / (double)(RAND_MAX+1.0) * n);
}

// Here we generate trips the same way the researches did in their implementation.
//______________________________________________________________________________________________________________________
int main() {

    printf("%u\n", TRIPS_AMOUNT);
    srand(1);
    for(unsigned int i = 0; i < TRIPS_AMOUNT; i++) {
        unsigned int s = randomNodeID(NODES_CNT);
        unsigned int t = randomNodeID(NODES_CNT);
        printf("%u %u\n", s, t);
    }

    return 0;
}

