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
package cz.cvut.fel.aic.sptests;

import static org.junit.jupiter.api.Assertions.assertEquals;

import cz.cvut.fel.aic.shortestpaths.TNRAFDistanceQueryManagerAPI;
import cz.cvut.fel.aic.shortestpaths.TNRDistanceQueryManagerAPI;
import cz.cvut.fel.aic.sptests.utils.Loader;
import cz.cvut.fel.aic.sptests.utils.Pair;
import cz.cvut.fel.aic.shortestpaths.CHDistanceQueryManagerAPI;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.io.FileNotFoundException;
import java.math.BigInteger;
import java.util.ArrayList;

// This class can be used to test that the API works correctly and one can call the library functions from Java.
// All tests are conducted on a graph of Prague that is precomputed for various methods (CH, TNR and TNRAF).
// The tests require you to have a "shortestPaths" library compiled for your architecture.
class ShortestPathsLibraryTests {

    // This is used as epsilon for comparing the expected and the returned distance.
    // Since we are working with doubles, small differences between values are possible. The true values were computed
    // using Dijkstra. The values computed using Contraction Hierarchies should not differ by more than the given
    // epsilon.
    double eps = 0.000000001;

    @Test
    @DisplayName("Test 1 - Contraction Hierarchies - 15 queries using original IDs - mapping required")
    void chTest1() {
        // This can be used if you can guarantee that the path to the library is always included in the
        // java.library.path. For example by setting -Djava.library.path to the directory with the library.
        // If you can not guarantee this, you can use an absolute path using System.load below.
        System.loadLibrary("shortestPaths");

        // Here you can put an absolute path to the library if you can't assure that you will have the library
        // included in the java.library.path.
        // System.load("/home/xenty/sum/2019/ContractionHierarchies/contraction-hierarchies/javatests/libcontractionHierarchies.so");

        CHDistanceQueryManagerAPI dqmm = new CHDistanceQueryManagerAPI();
        dqmm.initializeCH("./data/PragueCH.ch", "./data/PragueMapping.xeni");
        Loader l = new Loader();
        ArrayList<Pair<BigInteger, BigInteger>> testQueries = new ArrayList<Pair<BigInteger, BigInteger>>();
        ArrayList<Double> testDistances = new ArrayList<Double>();
        try {
            testQueries = l.loadQueriesBigInteger("./data/test15queries.txt");
            testDistances = l.loadTrueDistances("./data/test15reference.txt", testQueries.size());
        } catch (FileNotFoundException e) {
            System.out.println("Error reading input files for the test.");
            e.printStackTrace();
        }

        for (int i = 0; i < testQueries.size(); i++) {
            assertEquals(testDistances.get(i), dqmm.distanceQuery(testQueries.get(i).getElement0(), testQueries.get(i).getElement1()), eps);
        }

        dqmm.clearStructures();

    }

    @Test
    @DisplayName("Test 2 - Transit Node Routing - 15 queries using original IDs - mapping required")
    void tnrTest1() {
        // This can be used if you can guarantee that the path to the library is always included in the
        // java.library.path. For example by setting -Djava.library.path to the directory with the library.
        // If you can not guarantee this, you can use an absolute path using System.load below.
        System.loadLibrary("shortestPaths");

        // Here you can put an absolute path to the library if you can't assure that you will have the library
        // included in the java.library.path.
        // System.load("/home/xenty/sum/2019/ContractionHierarchies/contraction-hierarchies/javatests/libcontractionHierarchies.so");

        TNRDistanceQueryManagerAPI dqmm = new TNRDistanceQueryManagerAPI();
        dqmm.initializeTNR("./data/PragueTNR1000tnodes.tnrg", "./data/PragueMapping.xeni");
        Loader l = new Loader();
        ArrayList<Pair<BigInteger, BigInteger>> testQueries = new ArrayList<Pair<BigInteger, BigInteger>>();
        ArrayList<Double> testDistances = new ArrayList<Double>();
        try {
            testQueries = l.loadQueriesBigInteger("./data/test15queries.txt");
            testDistances = l.loadTrueDistances("./data/test15reference.txt", testQueries.size());
        } catch (FileNotFoundException e) {
            System.out.println("Error reading input files for the test.");
            e.printStackTrace();
        }

        for (int i = 0; i < testQueries.size(); i++) {
            assertEquals(testDistances.get(i), dqmm.distanceQuery(testQueries.get(i).getElement0(), testQueries.get(i).getElement1()), eps);
        }

        dqmm.clearStructures();

    }

    @Test
    @DisplayName("Test 3 - Transit Node Routing with Arc Flags - 15 queries using original IDs - mapping required")
    void tnrafTest1() {
        // This can be used if you can guarantee that the path to the library is always included in the
        // java.library.path. For example by setting -Djava.library.path to the directory with the library.
        // If you can not guarantee this, you can use an absolute path using System.load below.
        System.loadLibrary("shortestPaths");

        // Here you can put an absolute path to the library if you can't assure that you will have the library
        // included in the java.library.path.
        // System.load("/home/xenty/sum/2019/ContractionHierarchies/contraction-hierarchies/javatests/libcontractionHierarchies.so");

        TNRAFDistanceQueryManagerAPI dqmm = new TNRAFDistanceQueryManagerAPI();
        dqmm.initializeTNRAF("./data/PragueTNRAF1000tnodes.tgaf", "./data/PragueMapping.xeni");
        Loader l = new Loader();
        ArrayList<Pair<BigInteger, BigInteger>> testQueries = new ArrayList<Pair<BigInteger, BigInteger>>();
        ArrayList<Double> testDistances = new ArrayList<Double>();
        try {
            testQueries = l.loadQueriesBigInteger("./data/test15queries.txt");
            testDistances = l.loadTrueDistances("./data/test15reference.txt", testQueries.size());
        } catch (FileNotFoundException e) {
            System.out.println("Error reading input files for the test.");
            e.printStackTrace();
        }

        for (int i = 0; i < testQueries.size(); i++) {
            assertEquals(testDistances.get(i), dqmm.distanceQuery(testQueries.get(i).getElement0(), testQueries.get(i).getElement1()), eps);
        }

        dqmm.clearStructures();

    }


    @Test
    @DisplayName("Test 4 - Contraction Hierarchies - 5000 queries using original IDs - mapping required")
    void chTest2() {
        // This can be used if you can guarantee that the path to the library is always included in the
        // java.library.path. For example by setting -Djava.library.path to the directory with the library.
        // If you can not guarantee this, you can use an absolute path using System.load below.
        System.loadLibrary("shortestPaths");

        // Here you can put an absolute path to the library if you can't assure that you will have the library
        // included in the java.library.path.
        // System.load("/home/xenty/sum/2019/ContractionHierarchies/contraction-hierarchies/javatests/libcontractionHierarchies.so");

        CHDistanceQueryManagerAPI dqmm = new CHDistanceQueryManagerAPI();
        dqmm.initializeCH("./data/PragueCH.ch", "./data/PragueMapping.xeni");
        Loader l = new Loader();
        ArrayList<Pair<BigInteger, BigInteger>> testQueries = new ArrayList<Pair<BigInteger, BigInteger>>();
        ArrayList<Double> testDistances = new ArrayList<Double>();
        try {
            testQueries = l.loadQueriesBigInteger("./data/test5000queries.txt");
            testDistances = l.loadTrueDistances("./data/test5000reference.txt", testQueries.size());
        } catch (FileNotFoundException e) {
            System.out.println("Error reading input files for the test.");
            e.printStackTrace();
        }

        for (int i = 0; i < testQueries.size(); i++) {
            assertEquals(testDistances.get(i), dqmm.distanceQuery(testQueries.get(i).getElement0(), testQueries.get(i).getElement1()), eps);
        }

        dqmm.clearStructures();

    }

    @Test
    @DisplayName("Test 5 - Transit Node Routing - 5000 queries using original IDs - mapping required")
    void tnrTest2() {
        // This can be used if you can guarantee that the path to the library is always included in the
        // java.library.path. For example by setting -Djava.library.path to the directory with the library.
        // If you can not guarantee this, you can use an absolute path using System.load below.
        System.loadLibrary("shortestPaths");

        // Here you can put an absolute path to the library if you can't assure that you will have the library
        // included in the java.library.path.
        // System.load("/home/xenty/sum/2019/ContractionHierarchies/contraction-hierarchies/javatests/libcontractionHierarchies.so");

        TNRDistanceQueryManagerAPI dqmm = new TNRDistanceQueryManagerAPI();
        dqmm.initializeTNR("./data/PragueTNR1000tnodes.tnrg", "./data/PragueMapping.xeni");
        Loader l = new Loader();
        ArrayList<Pair<BigInteger, BigInteger>> testQueries = new ArrayList<Pair<BigInteger, BigInteger>>();
        ArrayList<Double> testDistances = new ArrayList<Double>();
        try {
            testQueries = l.loadQueriesBigInteger("./data/test5000queries.txt");
            testDistances = l.loadTrueDistances("./data/test5000reference.txt", testQueries.size());
        } catch (FileNotFoundException e) {
            System.out.println("Error reading input files for the test.");
            e.printStackTrace();
        }

        for (int i = 0; i < testQueries.size(); i++) {
            assertEquals(testDistances.get(i), dqmm.distanceQuery(testQueries.get(i).getElement0(), testQueries.get(i).getElement1()), eps);
        }

        dqmm.clearStructures();

    }

    @Test
    @DisplayName("Test 6 - Transit Node Routing with Arc Flags - 5000 queries using original IDs - mapping required")
    void tnrafTest2() {
        // This can be used if you can guarantee that the path to the library is always included in the
        // java.library.path. For example by setting -Djava.library.path to the directory with the library.
        // If you can not guarantee this, you can use an absolute path using System.load below.
        System.loadLibrary("shortestPaths");

        // Here you can put an absolute path to the library if you can't assure that you will have the library
        // included in the java.library.path.
        // System.load("/home/xenty/sum/2019/ContractionHierarchies/contraction-hierarchies/javatests/libcontractionHierarchies.so");

        TNRAFDistanceQueryManagerAPI dqmm = new TNRAFDistanceQueryManagerAPI();
        dqmm.initializeTNRAF("./data/PragueTNRAF1000tnodes.tgaf", "./data/PragueMapping.xeni");
        Loader l = new Loader();
        ArrayList<Pair<BigInteger, BigInteger>> testQueries = new ArrayList<Pair<BigInteger, BigInteger>>();
        ArrayList<Double> testDistances = new ArrayList<Double>();
        try {
            testQueries = l.loadQueriesBigInteger("./data/test5000queries.txt");
            testDistances = l.loadTrueDistances("./data/test5000reference.txt", testQueries.size());
        } catch (FileNotFoundException e) {
            System.out.println("Error reading input files for the test.");
            e.printStackTrace();
        }

        for (int i = 0; i < testQueries.size(); i++) {
            assertEquals(testDistances.get(i), dqmm.distanceQuery(testQueries.get(i).getElement0(), testQueries.get(i).getElement1()), eps);
        }

        dqmm.clearStructures();

    }
}
