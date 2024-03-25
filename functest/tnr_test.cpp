//
// Created by horakj31 on 2024-03-12.
//
#include "gtest/gtest.h"
#include "common.h"
#include "expected_graphs.h"

#include "GraphBuilding/Loaders/TNRGLoader.h"

TEST(tnr_test, from_xengraph1) {
    run_preprocessor("tnr xengraph fast 1 functest/01_xengraph.xeng from_xengraph1");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_xengraph1.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_1();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_xengraph2) {
    run_preprocessor("tnr xengraph slow 2 functest/01_xengraph.xeng from_xengraph2");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_xengraph2.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_2();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_xengraph3) {
    run_preprocessor("tnr xengraph dm 3 functest/01_xengraph.xeng from_xengraph3");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_xengraph3.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_3();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_xengraph4) {
    run_preprocessor("tnr xengraph fast 2 functest/02_xengraph.xeng from_xengraph4 100");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_xengraph4.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_02_2_div100();
    compare_tnr_graphs(*loaded, *expected);
}



TEST(tnr_test, from_dimacs1) {
    run_preprocessor("tnr dimacs fast 2 functest/01_dimacs.gr from_dimacs1");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_dimacs1.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_2();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_dimacs2) {
    run_preprocessor("tnr dimacs slow 3 functest/01_dimacs.gr from_dimacs2");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_dimacs2.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_3();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_dimacs3) {
    run_preprocessor("tnr dimacs dm 1 functest/01_dimacs.gr from_dimacs3");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_dimacs3.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_1();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_dimacs4) {
    run_preprocessor("tnr dimacs slow 2 functest/02_dimacs.gr from_dimacs4 100");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_dimacs4.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_02_2_div100();
    compare_tnr_graphs(*loaded, *expected);
}



TEST(tnr_test, from_csv1) {
    run_preprocessor("tnr csv fast 3 functest/01_adj.csv from_csv1");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_csv1.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_3();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_csv2) {
    run_preprocessor("tnr csv slow 1 functest/01_adj.csv from_csv2");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_csv2.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_1();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_csv3) {
    run_preprocessor("tnr csv dm 2 functest/01_adj.csv from_csv3");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_csv3.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_2();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_csv4) {
    run_preprocessor("tnr csv dm 2 functest/02_adj.csv from_csv4 100");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_csv4.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_02_2_div100();
    compare_tnr_graphs(*loaded, *expected);
}
