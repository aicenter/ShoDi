package cz.cvut.fel.aic.chtests.utils;

import java.io.File;
import java.io.FileNotFoundException;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Locale;
import java.util.Scanner;

public class Loader {
    public ArrayList<Double> loadTrueDistances(String filePath, int queriesCnt) throws FileNotFoundException {

        ArrayList<Double> retval = new ArrayList<Double>();
        Scanner scanner = new Scanner(new File(filePath));
        scanner.useLocale(Locale.US);
        for (int i = 0; i < queriesCnt; i++) {
            scanner.hasNextDouble();
            retval.add(scanner.nextDouble());
        }

        scanner.close();

        return retval;
    }

    public ArrayList<Pair<Integer, Integer>> loadQueries(String filePath) throws FileNotFoundException {
        ArrayList<Pair<Integer, Integer>> retval = new ArrayList<Pair<Integer, Integer>>();
        Scanner scanner = new Scanner(new File(filePath));
        int queriesCnt = scanner.nextInt();
        for (int i = 0; i < queriesCnt; i++) {
            int from = scanner.nextInt();
            int to = scanner.nextInt();
            retval.add(Pair.createPair(from, to));
        }

        scanner.close();

        return retval;
    }

    public ArrayList<Pair<BigInteger, BigInteger>> loadQueriesBigInteger(String filePath) throws FileNotFoundException {
        ArrayList<Pair<BigInteger, BigInteger>> retval = new ArrayList<Pair<BigInteger, BigInteger>>();
        Scanner scanner = new Scanner(new File(filePath));
        int queriesCnt = scanner.nextInt();
        for (int i = 0; i < queriesCnt; i++) {
            BigInteger from = scanner.nextBigInteger();
            BigInteger to = scanner.nextBigInteger();
            retval.add(Pair.createPair(from, to));
        }

        scanner.close();

        return retval;
    }

}
