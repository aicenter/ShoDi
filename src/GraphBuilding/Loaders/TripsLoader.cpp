//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "TripsLoader.h"
#include <fstream>

//______________________________________________________________________________________________________________________
TripsLoader::TripsLoader(string inputFile) {
    this->inputFile = inputFile;
}

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

