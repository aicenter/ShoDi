//
// Created by horakj31 on 2024-03-17.
//
#include "gtest/gtest.h"
#include "common.h"
#include "expected_graphs.h"

#include "GraphBuilding/Loaders/TGAFLoader.h"

TEST(tnraf_test, from_xengraph1) {
    run_preprocessor("tnraf xengraph slow 3 functest/01_xengraph.xeng from_xengraph1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
}

TEST(tnraf_test, from_xengraph2) {
    run_preprocessor("tnraf xengraph dm 2 functest/01_xengraph.xeng from_xengraph2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_2();
    compare_tnraf_graphs(*loaded, *expected);
}

TEST(tnraf_test, from_xengraph3) {
    run_preprocessor("tnraf xengraph dm 3 functest/02_xengraph.xeng from_xengraph3 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_xengraph3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
}



TEST(tnraf_test, from_dimacs1) {
    run_preprocessor("tnraf dimacs slow 2 functest/01_dimacs.gr from_dimacs1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_2();
    compare_tnraf_graphs(*loaded, *expected);
}

TEST(tnraf_test, from_dimacs2) {
    run_preprocessor("tnraf dimacs dm 1 functest/01_dimacs.gr from_dimacs2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
    compare_tnraf_graphs(*loaded, *expected);
}

TEST(tnraf_test, from_dimacs3) {
    run_preprocessor("tnraf dimacs slow 3 functest/02_dimacs.gr from_dimacs3 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_dimacs3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
}



TEST(tnraf_test, from_csv1) {
    run_preprocessor("tnraf csv slow 1 functest/01_adj.csv from_csv1");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv1.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_1();
    compare_tnraf_graphs(*loaded, *expected);
}

TEST(tnraf_test, from_csv2) {
    run_preprocessor("tnraf csv dm 3 functest/01_adj.csv from_csv2");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv2.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_01_3();
    compare_tnraf_graphs(*loaded, *expected);
}

TEST(tnraf_test, from_csv3) {
    run_preprocessor("tnraf csv dm 3 functest/02_adj.csv from_csv3 100");
    TransitNodeRoutingArcFlagsGraph* loaded = TGAFLoader("from_csv3.tgaf").loadTNRAFforDistanceQueries();
    TransitNodeRoutingArcFlagsGraph* expected = build_tnraf_graph_02_3_div100();
    compare_tnraf_graphs(*loaded, *expected);
}
