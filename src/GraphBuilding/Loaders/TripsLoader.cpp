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
// Created on: 8.7.19
//

#include "TripsLoader.h"
#include <fstream>

//______________________________________________________________________________________________________________________
TripsLoader::TripsLoader(std::string inputFile) {
    this->inputFile = inputFile;
}

//______________________________________________________________________________________________________________________
void TripsLoader::loadTrips(std::vector < std::pair< unsigned int, unsigned int > > & x) {
    std::ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    parseTrips(input, x);

    input.close();
}

//______________________________________________________________________________________________________________________
void TripsLoader::loadLongLongTrips(std::vector < std::pair< long long unsigned int, long long unsigned int > > & x) {
    std::ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    parseLongLongTrips(input, x);

    input.close();
}

//______________________________________________________________________________________________________________________
void TripsLoader::parseTrips(std::ifstream & input, std::vector < std::pair< unsigned int, unsigned int > > & x) {
    unsigned int tripscnt;
    input >> tripscnt;
    x.resize(tripscnt);
    for (unsigned int i = 0; i < tripscnt; i++) {
        input >> x.at(i).first >> x.at(i).second;
    }
}

//______________________________________________________________________________________________________________________
void TripsLoader::parseLongLongTrips(std::ifstream & input, std::vector < std::pair< long long unsigned int, long long unsigned int > > & x) {
    unsigned int tripscnt;
    input >> tripscnt;
    x.resize(tripscnt);
    for (unsigned int i = 0; i < tripscnt; i++) {
        input >> x.at(i).first >> x.at(i).second;
    }
}

