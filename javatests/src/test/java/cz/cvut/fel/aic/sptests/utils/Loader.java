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
package cz.cvut.fel.aic.sptests.utils;

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

    public ArrayList<Long> loadIntegerTrueDistances(String filePath, int queriesCnt) throws FileNotFoundException {

        ArrayList<Long> retval = new ArrayList<Long>();
        Scanner scanner = new Scanner(new File(filePath));
        scanner.useLocale(Locale.US);
        for (int i = 0; i < queriesCnt; i++) {
            scanner.hasNextDouble();
            retval.add(scanner.nextLong());
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
