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
#include "gtest/gtest.h"
#include "common.h"
#include "expected_graphs.h"
#include <cstdio>

#include "GraphBuilding/Loaders/TGAFLoader.h"

/**
 * FROM XENGRAPH TESTS
 */

/**
 * First set: --tnodes-cnt 3 -i functest/01_xengraph.
 */
/*TEST(tnraf_test, from_xengraph1_slow) {
    run_preprocessor("--method tnraf --input-format xengraph --preprocessing-mode slow --tnodes-cnt 3 -i functest/01_xengraph.xeng --output-path from_xengraph1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_xengraph1.tgaf");
}*/

TEST(tnraf_test, from_xengraph1_fast) {
    run_preprocessor("--method tnraf --input-format xengraph --preprocessing-mode fast --tnodes-cnt 3 -i functest/01_xengraph.xeng --output-path from_xengraph1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_xengraph1.tgaf");
}

TEST(tnraf_test, from_xengraph1_dm) {
    run_preprocessor("--method tnraf --input-format xengraph --preprocessing-mode dm --tnodes-cnt 3 -i functest/01_xengraph.xeng --output-path from_xengraph1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_xengraph1.tgaf");
}


/**
 * Second set: --tnodes-cnt 2 -i functest/01_xengraph.
 */
/*TEST(tnraf_test, from_xengraph2_slow) {
    run_preprocessor("-m tnraf --preprocessing-mode slow --tnodes-cnt 2 --input-path functest/01_xengraph.xeng --output-path from_xengraph2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_2();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_xengraph2.tgaf");
}*/

TEST(tnraf_test, from_xengraph2_fast) {
    run_preprocessor("-m tnraf --preprocessing-mode fast --tnodes-cnt 2 --input-path functest/01_xengraph.xeng --output-path from_xengraph2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_2();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_xengraph2.tgaf");
}

TEST(tnraf_test, from_xengraph2_dm) {
    run_preprocessor("-m tnraf --preprocessing-mode dm --tnodes-cnt 2 --input-path functest/01_xengraph.xeng --output-path from_xengraph2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_2();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_xengraph2.tgaf");
}

/**
 * Third set: --tnodes-cnt 3 -i functest/02_xengraph.
 */
/*TEST(tnraf_test, from_xengraph3_slow) {
    run_preprocessor("--method tnraf --input-format xengraph --preprocessing-mode slow --tnodes-cnt 3 -i functest/02_xengraph.xeng --output-path from_xengraph3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_xengraph3.tgaf");
}*/

TEST(tnraf_test, from_xengraph3_fast) {
    run_preprocessor("--method tnraf --input-format xengraph --preprocessing-mode fast --tnodes-cnt 3 -i functest/02_xengraph.xeng --output-path from_xengraph3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_xengraph3.tgaf");
}

TEST(tnraf_test, from_xengraph3_dm) {
    run_preprocessor("--method tnraf --input-format xengraph --preprocessing-mode dm --tnodes-cnt 3 -i functest/02_xengraph.xeng --output-path from_xengraph3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_xengraph3.tgaf");
}



/**
 * DIMACS tests
 */

/**
 * First set: --tnodes-cnt 2 -i functest/01_dimacs.gr
 */
/*TEST(tnraf_test, from_dimacs1_slow) {
    run_preprocessor("--method tnraf --preprocessing-mode slow --tnodes-cnt 2 -i functest/01_dimacs.gr --output-path from_dimacs1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_2();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_dimacs1.tgaf");
}*/

TEST(tnraf_test, from_dimacs1_fast) {
    run_preprocessor("--method tnraf --preprocessing-mode fast --tnodes-cnt 2 -i functest/01_dimacs.gr --output-path from_dimacs1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_2();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_dimacs1.tgaf");
}

TEST(tnraf_test, from_dimacs1_dm) {
    run_preprocessor("--method tnraf --preprocessing-mode dm --tnodes-cnt 2 -i functest/01_dimacs.gr --output-path from_dimacs1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_2();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_dimacs1.tgaf");
}


/**
 * Second set: --tnodes-cnt 1 -i functest/01_dimacs.gr
 */
// TEST(tnraf_test, from_dimacs2_slow) {
//     run_preprocessor("--method tnraf --input-format dimacs --preprocessing-mode slow --tnodes-cnt 1 --input-path functest/01_dimacs.gr --output-path from_dimacs2");
//     TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs2.tgaf").loadTNRAFforDistanceQueries();
//     TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
//     compare_tnraf_graphs(*loaded, *expected);
//     delete loaded;
//     delete expected;
//     std::remove("from_dimacs2.tgaf");
// }

TEST(tnraf_test, from_dimacs2_fast) {
    run_preprocessor("--method tnraf --input-format dimacs --preprocessing-mode fast --tnodes-cnt 1 --input-path functest/01_dimacs.gr --output-path from_dimacs2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_dimacs2.tgaf");
}

TEST(tnraf_test, from_dimacs2_dm) {
    run_preprocessor("--method tnraf --input-format dimacs --preprocessing-mode dm --tnodes-cnt 1 --input-path functest/01_dimacs.gr --output-path from_dimacs2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_dimacs2.tgaf");
}


/**
 * Third set: --tnodes-cnt 3 -i functest/02_dimacs.gr
 */
/*TEST(tnraf_test, from_dimacs3_slow) {
    run_preprocessor("--method tnraf --input-format dimacs --preprocessing-mode slow --tnodes-cnt 3 -i functest/02_dimacs.gr -o from_dimacs3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_dimacs3.tgaf");
}*/

TEST(tnraf_test, from_dimacs3_fast) {
    run_preprocessor("--method tnraf --input-format dimacs --preprocessing-mode fast --tnodes-cnt 3 -i functest/02_dimacs.gr -o from_dimacs3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_dimacs3.tgaf");
}

TEST(tnraf_test, from_dimacs3_dm) {
    run_preprocessor("--method tnraf --input-format dimacs --preprocessing-mode dm --tnodes-cnt 3 -i functest/02_dimacs.gr -o from_dimacs3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_dimacs3.tgaf");
}



/**
 * ADJ tests
 */

/**
 * First set: --tnodes-cnt 1 -i functest/01_adj.csv
 */
/*TEST(tnraf_test, from_adj1_slow) {
    run_preprocessor("--method tnraf --input-format adj --preprocessing-mode slow --tnodes-cnt 1 -i functest/01_adj.csv --output-path from_adj1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_adj1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_adj1.tgaf");
}*/

TEST(tnraf_test, from_adj1_fast) {
    run_preprocessor("--method tnraf --input-format adj --preprocessing-mode fast --tnodes-cnt 1 -i functest/01_adj.csv --output-path from_adj1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_adj1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_adj1.tgaf");
}

TEST(tnraf_test, from_adj1_dm) {
    run_preprocessor("--method tnraf --input-format adj --preprocessing-mode dm --tnodes-cnt 1 -i functest/01_adj.csv --output-path from_adj1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_adj1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_adj1.tgaf");
}


/**
 * Second set: --tnodes-cnt 3 -i functest/01_adj.csv
 */
/*TEST(tnraf_test, from_adj2_slow) {
    run_preprocessor("--method tnraf --input-format adj --preprocessing-mode slow --tnodes-cnt 3 --input-path functest/01_adj.csv -o from_adj2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_adj2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_adj2.tgaf");
}*/

TEST(tnraf_test, from_adj2_fast) {
    run_preprocessor("--method tnraf --input-format adj --preprocessing-mode fast --tnodes-cnt 3 --input-path functest/01_adj.csv -o from_adj2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_adj2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_adj2.tgaf");
}

TEST(tnraf_test, from_adj2_dm) {
    run_preprocessor("--method tnraf --preprocessing-mode dm --tnodes-cnt 3 --input-path functest/01_adj.csv -o from_adj2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_adj2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_adj2.tgaf");
}


/**
 * Third set: --tnodes-cnt 3 -i functest/02_adj.csv
 */
/*TEST(tnraf_test, from_adj3_slow) {
    run_preprocessor("-m tnraf --preprocessing-mode slow --input-format adj --tnodes-cnt 3 --input-path functest/02_adj.csv --output-path from_adj3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_adj3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_adj3.tgaf");
}*/

TEST(tnraf_test, from_adj3_fast) {
    run_preprocessor("-m tnraf --preprocessing-mode fast --input-format adj --tnodes-cnt 3 --input-path functest/02_adj.csv --output-path from_adj3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_adj3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_adj3.tgaf");
}

TEST(tnraf_test, from_adj3_dm) {
    run_preprocessor("-m tnraf --preprocessing-mode dm --input-format adj --tnodes-cnt 3 --input-path functest/02_adj.csv --output-path from_adj3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_adj3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_adj3.tgaf");
}



/**
 * CSV tests
 */

/**
 * First set: --tnodes-cnt 1 -i functest/01_csv
 */
/*TEST(tnraf_test, from_csv1_slow) {
    run_preprocessor("--method tnraf --input-format csv --preprocessing-mode slow --tnodes-cnt 1 -i functest/01_csv --output-path from_csv1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_csv1.tgaf");
}*/

TEST(tnraf_test, from_csv1_fast) {
    run_preprocessor("--method tnraf --input-format csv --preprocessing-mode fast --tnodes-cnt 1 -i functest/01_csv --output-path from_csv1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_csv1.tgaf");
}

TEST(tnraf_test, from_csv1_dm) {
    run_preprocessor("--method tnraf --input-format csv --preprocessing-mode dm --tnodes-cnt 1 -i functest/01_csv --output-path from_csv1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_csv1.tgaf");
}


/**
 * Second set: --tnodes-cnt 3 -i functest/01_csv
 */
/*TEST(tnraf_test, from_csv2_slow) {
    run_preprocessor("--method tnraf -f csv --preprocessing-mode slow --tnodes-cnt 3 --input-path functest/01_csv --output-path from_csv2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_csv2.tgaf");
}*/

TEST(tnraf_test, from_csv2_fast) {
    run_preprocessor("--method tnraf -f csv --preprocessing-mode fast --tnodes-cnt 3 --input-path functest/01_csv --output-path from_csv2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_csv2.tgaf");
}

TEST(tnraf_test, from_csv2_dm) {
    run_preprocessor("--method tnraf -f csv --preprocessing-mode dm --tnodes-cnt 3 --input-path functest/01_csv --output-path from_csv2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_csv2.tgaf");
}


/**
 * Third set: --tnodes-cnt 3 -i functest/02_csv
 */
/*TEST(tnraf_test, from_csv3_slow) {
    run_preprocessor("-m tnraf --preprocessing-mode slow --input-format csv --tnodes-cnt 3 -i functest/02_csv -o from_csv3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_csv3.tgaf");
}*/

TEST(tnraf_test, from_csv3_fast) {
    run_preprocessor("-m tnraf --preprocessing-mode fast --input-format csv --tnodes-cnt 3 -i functest/02_csv -o from_csv3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_csv3.tgaf");
}

TEST(tnraf_test, from_csv3_dm) {
    run_preprocessor("-m tnraf --preprocessing-mode dm --tnodes-cnt 3 -i functest/02_csv -o from_csv3 --precision-loss 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
    delete loaded;
    delete expected;
    std::remove("from_csv3.tgaf");
}
