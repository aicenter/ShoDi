//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "TripsLoader.h"
#include <fstream>

TripsLoader::TripsLoader(string inputFile) {
    this->inputFile = inputFile;
}

// This function just loads trips in a simple format.
// The trips file should be a text file starting with an unsigned int with the number of trips "n" and then
// "n" rows should follow each row containing two integers "source" and "target" separated by a space.
// This function doesn't validate that the sources and targets are valid (that means that they are lower number than
// the amount of edges in the used graph), it just loads the pairs. Do any validations yourself, if necessary.
//______________________________________________________________________________________________________________________
void TripsLoader::loadTrips(vector < pair < unsigned int, unsigned int > > & x) {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    parseTrips(input, x);

    input.close();
}

//______________________________________________________________________________________________________________________
void TripsLoader::loadLongLongTrips(vector < pair < long long unsigned int, long long unsigned int > > & x) {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    parseLongLongTrips(input, x);

    input.close();
}

//______________________________________________________________________________________________________________________
void TripsLoader::parseTrips(ifstream & input, vector < pair < unsigned int, unsigned int > > & x) {
    unsigned int tripscnt;
    input >> tripscnt;
    x.resize(tripscnt);
    for (unsigned int i = 0; i < tripscnt; i++) {
        input >> x.at(i).first >> x.at(i).second;
    }
}

//______________________________________________________________________________________________________________________
void TripsLoader::parseLongLongTrips(ifstream & input, vector < pair < long long unsigned int, long long unsigned int > > & x) {
    unsigned int tripscnt;
    input >> tripscnt;
    x.resize(tripscnt);
    for (unsigned int i = 0; i < tripscnt; i++) {
        input >> x.at(i).first >> x.at(i).second;
    }
}

