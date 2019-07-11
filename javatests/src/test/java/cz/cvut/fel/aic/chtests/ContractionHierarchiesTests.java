package cz.cvut.fel.aic.chtests;

import static org.junit.jupiter.api.Assertions.assertEquals;

import cz.cvut.fel.aic.chtests.utils.Loader;
import cz.cvut.fel.aic.chtests.utils.Pair;
import cz.cvut.fel.aic.contractionhierarchies.DistanceQueryManagerAPI;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.io.FileNotFoundException;
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

        DistanceQueryManagerAPI dqm = new DistanceQueryManagerAPI();
        dqm.initializeCH("./testCHGraph.chf");
        Loader l = new Loader();
        ArrayList<Pair<Integer, Integer>> testQueries = new ArrayList<Pair<Integer, Integer>>();
        ArrayList<Double> testDistances = new ArrayList<Double>();
        try {
            testQueries = l.loadQueries("./testQueries.txt");
            testDistances = l.loadTrueDistances("./testTrueDistances.txt", testQueries.size());
        } catch (FileNotFoundException e) {
            System.out.println("Error reading input files for the test.");
            e.printStackTrace();
        }

        for(int i = 0; i < testQueries.size(); i++) {
            assertEquals(testDistances.get(i), dqm.distanceQuery(testQueries.get(i).getElement0(), testQueries.get(i).getElement1()), eps);
        }

        dqm.clearStructures();

    }
}
