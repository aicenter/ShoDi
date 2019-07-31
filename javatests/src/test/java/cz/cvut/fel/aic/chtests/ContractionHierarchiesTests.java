package cz.cvut.fel.aic.chtests;

import static org.junit.jupiter.api.Assertions.assertEquals;

import cz.cvut.fel.aic.chtests.utils.Loader;
import cz.cvut.fel.aic.chtests.utils.Pair;
import cz.cvut.fel.aic.contractionhierarchies.IntegerDistanceQueryManagerAPI;
import cz.cvut.fel.aic.contractionhierarchies.FPointDistanceQueryManagerAPI;
import cz.cvut.fel.aic.contractionhierarchies.FPointDistanceQueryManagerWithMappingAPI;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.io.FileNotFoundException;
import java.math.BigInteger;
import java.util.ArrayList;

class ContractionHierarchiesTests {

    // This is used as epsilon for comparing the expected and the returned distance.
    // Since we are working with doubles, small differences between values are possible. The true values were computed
    // using Dijkstra. The values computed using Contraction Hierarchies should not differ by more than the given
    // epsilon.
    double eps = 0.000000001;

    @Test
    @DisplayName("1000 random queries")
    void randomQueries1000Test() {
        // This can be used if you can guarantee that the path to the library is always included in the
        // java.library.path. For example by setting -Djava.library.path to the directory with the library.
        // If you can not guarantee this, you can use an absolute path using System.load below.
        System.loadLibrary("contractionHierarchies");

        // Here you can put an absolute path to the library if you can't assure that you will have the library
        // included in the java.library.path.
        // System.load("/home/xenty/sum/2019/ContractionHierarchies/contraction-hierarchies/javatests/libcontractionHierarchies.so");

        FPointDistanceQueryManagerAPI dqm = new FPointDistanceQueryManagerAPI();
        dqm.initializeCH("./data/testCHGraph.chf");
        Loader l = new Loader();
        ArrayList<Pair<Integer, Integer>> testQueries = new ArrayList<Pair<Integer, Integer>>();
        ArrayList<Double> testDistances = new ArrayList<Double>();
        try {
            testQueries = l.loadQueries("./data/testQueries.txt");
            testDistances = l.loadTrueDistances("./data/testTrueDistances.txt", testQueries.size());
        } catch (FileNotFoundException e) {
            System.out.println("Error reading input files for the test.");
            e.printStackTrace();
        }

        for(int i = 0; i < testQueries.size(); i++) {
            assertEquals(testDistances.get(i), dqm.distanceQuery(testQueries.get(i).getElement0(), testQueries.get(i).getElement1()), eps);
        }

        dqm.clearStructures();

    }

    @Test
    @DisplayName("15 queries using original IDs - mapping required")
    void mappingTripsTest() {
        // This can be used if you can guarantee that the path to the library is always included in the
        // java.library.path. For example by setting -Djava.library.path to the directory with the library.
        // If you can not guarantee this, you can use an absolute path using System.load below.
        System.loadLibrary("contractionHierarchies");

        // Here you can put an absolute path to the library if you can't assure that you will have the library
        // included in the java.library.path.
        // System.load("/home/xenty/sum/2019/ContractionHierarchies/contraction-hierarchies/javatests/libcontractionHierarchies.so");

        FPointDistanceQueryManagerWithMappingAPI dqmm = new FPointDistanceQueryManagerWithMappingAPI();
        dqmm.initializeCH("./data/testCHGraph.chf", "./data/testGraphMapping.xeni");
        Loader l = new Loader();
        ArrayList<Pair<BigInteger, BigInteger>> testQueries = new ArrayList<Pair<BigInteger, BigInteger>>();
        ArrayList<Double> testDistances = new ArrayList<Double>();
        try {
            testQueries = l.loadQueriesBigInteger("./data/testQueriesOriginalIDs.txt");
            testDistances = l.loadTrueDistances("./data/testTrueDistancesMapping.txt", testQueries.size());
        } catch (FileNotFoundException e) {
            System.out.println("Error reading input files for the test.");
            e.printStackTrace();
        }

        for(int i = 0; i < testQueries.size(); i++) {
            assertEquals(testDistances.get(i), dqmm.distanceQuery(testQueries.get(i).getElement0(), testQueries.get(i).getElement1()), eps);
        }

        dqmm.clearStructures();

    }

    @Test
    @DisplayName("Integer Contraction Hierarchy for Prague - 5000 random trips")
    void integerCHPrague5000randomTripsTest() {
        // This can be used if you can guarantee that the path to the library is always included in the
        // java.library.path. For example by setting -Djava.library.path to the directory with the library.
        // If you can not guarantee this, you can use an absolute path using System.load below.
        System.loadLibrary("contractionHierarchies");

        // Here you can put an absolute path to the library if you can't assure that you will have the library
        // included in the java.library.path.
        // System.load("/home/xenty/sum/2019/ContractionHierarchies/contraction-hierarchies/javatests/libcontractionHierarchies.so");

        IntegerDistanceQueryManagerAPI dqm = new IntegerDistanceQueryManagerAPI();
        dqm.initializeCH("./data/Prague_map_int.ch");
        Loader l = new Loader();
        ArrayList<Pair<Integer, Integer>> testQueries = new ArrayList<Pair<Integer, Integer>>();
        ArrayList<Long> testDistances = new ArrayList<Long>();
        try {
            testQueries = l.loadQueries("./data/Prague_map_5000randomTrips.txt");
            testDistances = l.loadIntegerTrueDistances("./data/Prague_map_5000trueDistances.txt", testQueries.size());
        } catch (FileNotFoundException e) {
            System.out.println("Error reading input files for the test.");
            e.printStackTrace();
        }

        for(int i = 0; i < testQueries.size(); i++) {
            assertEquals(testDistances.get(i), dqm.distanceQuery(testQueries.get(i).getElement0(), testQueries.get(i).getElement1()));
        }

        dqm.clearStructures();

    }
}
