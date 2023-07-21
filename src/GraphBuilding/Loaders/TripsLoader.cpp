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

