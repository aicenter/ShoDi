//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_TRIPSLOADER_H
#define CONTRACTION_HIERARCHIES_TRIPSLOADER_H

#include <string>
#include <vector>

using namespace std;

// This class is used to load trips which can then be used to test the hierarchy either to measure time or validate
// that it gives correct results by comparing the computed distances with distances computed by Dijkstra, which
// is also implemented in this project.
// The trips files should have a simple format starting with 'n' the number of trips followed by 'n' pairs of ID pairs
// source -> target. In the case of loadLongLongTrips(), original IDs from the .geojson files can be used instead of
// artificial indices, query managers with mapping must be used then.
//______________________________________________________________________________________________________________________
class TripsLoader {
private:
    string inputFile;
    void parseTrips(ifstream & input, vector < pair < unsigned int, unsigned int > > & x);
    void parseLongLongTrips(ifstream & input, vector < pair < long long unsigned int, long long unsigned int > > & x);
public:
    TripsLoader(string inputFile);
    void loadTrips(vector < pair < unsigned int, unsigned int > > & x);
    void loadLongLongTrips(vector < pair < long long unsigned int, long long unsigned int > > & x);
};


#endif //CONTRACTION_HIERARCHIES_TRIPSLOADER_H
