//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_TRIPSLOADER_H
#define CONTRACTION_HIERARCHIES_TRIPSLOADER_H

#include <string>
#include <vector>

using namespace std;

/**
 * This class is used to load trips which can then be used to test the hierarchy either to measure time or validate
 * that it gives correct results by comparing the computed distances with distances computed by Dijkstra, which
 * is also implemented in this project.
 * The trips files should have a simple format starting with 'n' the number of trips followed by 'n' pairs of ID pairs
 * source -> target. In the case of loadLongLongTrips(), original IDs from the .geojson files can be used instead of
 * artificial indices, query managers with mapping must be used then.
 */
class TripsLoader {
private:
    string inputFile;

    /**
     * Auxiliary function used to parse the queries from the input file.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param x[out] The vector that will be filled with the queries.
     */
    void parseTrips(ifstream & input, vector < pair < unsigned int, unsigned int > > & x);

    /**
     * Auxiliary function used to parse the queries from the input file.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param x[out] The vector that will be filled with the queries.
     */
    void parseLongLongTrips(ifstream & input, vector < pair < long long unsigned int, long long unsigned int > > & x);
public:
    /**
     * A simple constructor.
     *
     * @param inputFile[in] A path towards a file that should be loaded using this loader.
     */
    explicit TripsLoader(string inputFile);

    /**
     * This function just loads trips in a simple format.
     * The trips file should be a text file starting with an unsigned int with the number of trips "n" and then
     * "n" rows should follow each row containing two integers "source" and "target" separated by a space.
     * This function doesn't validate that the sources and targets are valid (that means that they are lower number than
     * the amount of edges in the used graph), it just loads the pairs. Do any validations yourself, if necessary.
     *
     * @param x[out] The vector that will be filled with the query pairs. The vector need to be resized before calling
     * this function to be able to contain all the queries.
     */
    void loadTrips(vector < pair < unsigned int, unsigned int > > & x);

    /**
     * Loads the queries in a simple format.
     * The trips file should be formatted in the same way as for the 'loadTrips' function, but in this case the two
     * integers on each line can be 64 bit unsigned integers. This variant can be used when using mapping.
     * This function doesn't validate that the sources and targets are valid (that means that the nodes with the given
     * IDs actually exist in the original graph), it just loads the pairs. Do any validations yourself, if necessary.
     *
     * @param x[out] The vector that will be filled with the query pairs. The vector need to be resized before calling
     * this function to be able to contain all the queries.
     */
    void loadLongLongTrips(vector < pair < long long unsigned int, long long unsigned int > > & x);
};


#endif //CONTRACTION_HIERARCHIES_TRIPSLOADER_H
