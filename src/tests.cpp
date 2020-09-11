//
// Author: Xenty (Michal Cvach)
// Created on: 27.7.20
//

#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory>
#include "constants.h"
#include "Timer/Timer.h"
#include "GraphBuilding/Loaders/XenGraphLoader.h"
#include "DistanceMatrix/DistanceMatrixComputorSlow.h"
#include "GraphBuilding/Loaders/TripsLoader.h"
#include "Benchmarking/DistanceMatrixBenchmark.h"
#include "Benchmarking/CorrectnessValidator.h"
#include "DistanceMatrix/DistanceMatrixComputorFast.h"

using namespace std;

/**
 * Precomputes a full Distance Matrix for a graph contained in a given file. Returns the obtained Distance Matrix
 * immediately (without outputting it to a file).
 *
 * @param inputFilePath[in] The path to the file containing the graph
 * @return An instance of the DistanceMatrix class containing shortest distances for all pairs of nodes.
 */
DistanceMatrix *obtainDM(GraphLoader &loader, DistanceMatrixComputor &computor) {
    Timer timer("Whole Distance Matrix computation timer");
    timer.begin();

    computor.computeDistanceMatrix(loader);

    timer.finish();
    timer.printMeasuredTime();

    return computor.getDistanceMatrixInstance();
}

/**
 * Auxiliary function than can load the true (expected) values for a set of queries and store them in a vector.
 *
 * @param trueValues[out] The vector that the true values will be loaded into.
 * @param inputFilePath[in] The file path to the text file containing the true (expected) values.
 */
void obtainTrueValues(vector<unsigned int> &trueValues, const string &inputFilePath) {
    ifstream input;
    input.open(inputFilePath);
    unsigned int queriesCnt;
    input >> queriesCnt;
    trueValues.resize(queriesCnt);
    for(size_t i = 0; i < queriesCnt; ++i) {
        input >> trueValues[i];
    }
}

bool validateDM(GraphLoader &graphLoader, DistanceMatrixComputor &computor, string tripsFilePath,
                string trueDistancesFilePath) {
    DistanceMatrix * dm = obtainDM(graphLoader, computor);

    TripsLoader querySetLoader = TripsLoader(tripsFilePath);
    vector<pair<unsigned int, unsigned int>> querySet;
    querySetLoader.loadTrips(querySet);

    vector<dist_t> trueValues;
    obtainTrueValues(trueValues, trueDistancesFilePath);

    vector<dist_t> dmDistances(querySet.size());
    DistanceMatrixBenchmark::benchmark(querySet, *dm, dmDistances);

    delete dm;
    return CorrectnessValidator::validate(trueValues, dmDistances);
}

struct Test {
    const string name;
    bool (*const func)();

    Test(string name, bool (*func)()) : name(name), func(func) {}
};

void runTests(std::initializer_list<Test> tests) {
    unsigned int successful = 0;
    unsigned int failed = 0;

    for (const Test &test : tests) {
        const bool result = test.func();
        cout << test.name << ": ";

        if (result) {
            cout << "SUCCESS";
            ++successful;
        } else {
            cout << "FAILED";
            ++failed;
        }
        cout << endl;
    }

    cout << successful << '/' << (failed + successful) << " tests were successful." << endl;

    if (failed > 0) {
        cout << failed << " tests FAILED." << endl;
    }
}

/**
 * This main function could serve as a simple test suite. In the future, using a C++ test framework such as Catch,
 * Boost.Test or Google Test would probably be helpful.
 */
int main() {
    setbuf(stdout, NULL);

    /*
     * A small graph containing 260 nodes and 561 edges (obtained from a real road network corresponding to
     * a part of Czech Budweis) is used here. All pairs of nodes are tried in this test.
     */

    const Test smallSlowTest = Test("Small graph (slow)", []() {
        XenGraphLoader gl("../testdata/BudweisCenter/BudweisCenter.xeng");
        DistanceMatrixComputorSlow dmc;
        return validateDM(gl, dmc, "../testdata/BudweisCenter/BudweisCenterAllPairsTrips.txt", "../testdata/BudweisCenter/BudweisCenterTrueDistances.txt");
    });

    const Test smallFastTest = Test("Small graph (fast)", []() {
        XenGraphLoader gl("../testdata/BudweisCenter/BudweisCenter.xeng");
        DistanceMatrixComputorFast dmc;
        return validateDM(gl, dmc, "../testdata/BudweisCenter/BudweisCenterAllPairsTrips.txt", "../testdata/BudweisCenter/BudweisCenterTrueDistances.txt");
    });

    /*
     * A graph of Prague containing 28686 nodes and 68331 edges is used here.
     * A query set containing 100000 random queries is used.
     */
    const Test mediumSlowTest = Test("Medium graph (slow)", []() {
        XenGraphLoader gl("../testdata/Prague/Prague.xeng");
        DistanceMatrixComputorSlow dmc;
        return validateDM(gl, dmc, "../testdata/Prague/Prague100000randomQueries.txt", "../testdata/Prague/PragueTrueDistances.txt");
    });

    const Test mediumFastTest = Test("Medium graph (fast)", []() {
        XenGraphLoader gl("../testdata/Prague/Prague.xeng");
        DistanceMatrixComputorFast dmc;
        return validateDM(gl, dmc, "../testdata/Prague/Prague100000randomQueries.txt", "../testdata/Prague/PragueTrueDistances.txt");
    });

    runTests({
        smallSlowTest,
        smallFastTest,
        mediumSlowTest,
        mediumFastTest
    });

    return 0;
}
