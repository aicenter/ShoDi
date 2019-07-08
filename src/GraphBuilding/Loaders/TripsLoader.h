//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_TRIPSLOADER_H
#define CONTRACTION_HIERARCHIES_TRIPSLOADER_H

#include <string>
#include <vector>

using namespace std;

class TripsLoader {
private:
    string inputFile;
    void parseTrips(ifstream & input, vector < pair < unsigned int, unsigned int > > & x);
public:
    TripsLoader(string inputFile);
    void loadTrips(vector < pair < unsigned int, unsigned int > > & x);
};


#endif //CONTRACTION_HIERARCHIES_TRIPSLOADER_H
