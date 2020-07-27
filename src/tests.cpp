//
// Author: Xenty (Michal Cvach)
// Created on: 27.7.20
//

#include <cstdio>
#include <iostream>
#include <fstream>
#include "Timer/Timer.h"
#include "GraphBuilding/Loaders/XenGraphLoader.h"
#include "DistanceMatrix/DistanceMatrixComputor.h"
#include "GraphBuilding/Loaders/TripsLoader.h"
#include "Benchmarking/DistanceMatrixBenchmark.h"
#include "Benchmarking/CorrectnessValidator.h"

using namespace std;

/**
 * Precomputes a full Distance Matrix for a graph contained in a given file. Returns the obtained Distance Matrix
 * immediately (without outputting it to a file).
 *
 * @param inputFilePath[in] The path to the file containing the graph
 * @return An instance of the DistanceMatrix class containing shortest distances for all pairs of nodes.
 */
DistanceMatrix * obtainDM(char const * inputFilePath) {
    Timer timer("Whole Distance Matrix computation timer");
    timer.begin();

    XenGraphLoader dijkstraGraphLoader = XenGraphLoader(inputFilePath);
    Graph * graph = dijkstraGraphLoader.loadGraph();

    DistanceMatrixComputor dmComputor;
    dmComputor.computeDistanceMatrix(*graph);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;

    return dmComputor.getDistanceMatrixInstance();
}

/**
 * Auxiliary function than can load the true (expected) values for a set of queries and store them in a vector.
 *
 * @param trueValues[out] The vector that the true values will be loaded into.
 * @param inputFilePath[in] The file path to the text file containing the true (expected) values.
 */
void obtainTrueValues(vector<unsigned int> & trueValues, const string & inputFilePath) {
    ifstream input;
    input.open(inputFilePath);
    unsigned int queriesCnt;
    input >> queriesCnt;
    trueValues.resize(queriesCnt);
    for(unsigned int i = 0; i < queriesCnt; ++i) {
        input >> trueValues[i];
    }
}

/**
 * A simple test validating that the Distance Matrix precomputed using the DistanceMatrixComputor returns correct
 * distances. A small graph containing 260 nodes and 561 edges (obtained from a real road network corresponding to
 * a part of Czech Budweis) is used here. All pairs of nodes are tried in this test.
 *
 * @return Returns 0 if the test executes successfully. Returns 1 in case of failure.
 */
int validateDMSmallGraph() {
    DistanceMatrix * dm = obtainDM("../testdata/BudweisCenter/BudweisCenter.xeng");

    TripsLoader querySetLoader = TripsLoader("../testdata/BudweisCenter/BudweisCenterAllPairsTrips.txt");
    vector<pair<unsigned int, unsigned int>> querySet;
    querySetLoader.loadTrips(querySet);

    vector<unsigned int> trueValues;
    obtainTrueValues(trueValues, "../testdata/BudweisCenter/BudweisCenterTrueDistances.txt");

    vector<unsigned int> dmDistances(querySet.size());
    double time = DistanceMatrixBenchmark::benchmark(querySet, *dm, dmDistances);

    if (CorrectnessValidator::validate(trueValues, dmDistances) == false) {
        return 1;
    }

    cout << "DM correctly answered " << querySet.size() << " queries in " << time << " seconds." << endl;
    return 0;
}

/**
 * A simple test validating that the Distance Matrix precomputed using the DistanceMatrixComputor returns correct
 * distances. A graph of Prague containing 28686 nodes and 68331 edges is used here.
 * A query set containing 100000 random queries is used.
 *
 * @return Returns 0 if the test executes successfully. Returns 1 in case of failure.
 */
int validateDMMediumGraph() {
    DistanceMatrix * dm = obtainDM("../testdata/Prague/Prague.xeng");

    TripsLoader querySetLoader = TripsLoader("../testdata/Prague/Prague100000randomQueries.txt");
    vector<pair<unsigned int, unsigned int>> querySet;
    querySetLoader.loadTrips(querySet);

    vector<unsigned int> trueValues;
    obtainTrueValues(trueValues, "../testdata/Prague/PragueTrueDistances.txt");

    vector<unsigned int> dmDistances(querySet.size());
    double time = DistanceMatrixBenchmark::benchmark(querySet, *dm, dmDistances);

    if (CorrectnessValidator::validate(trueValues, dmDistances) == false) {
        return 1;
    }

    cout << "DM correctly answered " << querySet.size() << " queries in " << time << " seconds." << endl;
    return 0;
}

/**
 * This main function could serve as a simple test suite. In the future, using a C++ test framework such as Catch,
 * Boost.Test or Google Test would probably be helpful.
 */
int main(int argc, char * argv[]) {
    setbuf(stdout, NULL);
    unsigned int failedTests = 0;

    failedTests += validateDMSmallGraph();
    //failedTests += validateDMMediumGraph();

    if(failedTests == 0) {
        cout << "All tests finished successfully." << endl;
    } else {
        cout << failedTests << " tests failed." << endl;
    }

    return 0;
}