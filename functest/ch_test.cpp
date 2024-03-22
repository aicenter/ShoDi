//
// Created by horakj31 on 2024-03-01.
//
#include "gtest/gtest.h"
#include "common.h"

#include "GraphBuilding/Loaders/DDSGLoader.h"

FlagsGraph* build_flags_graph_01() {
    FlagsGraph* g = new FlagsGraph(4);

    g->data(0).rank = 2;
    g->data(1).rank = 3;
    g->data(2).rank = 1;
    g->data(3).rank = 4;

    g->addEdge(0, 1, 1, true, false);
    g->addEdge(2, 0, 2, false, true);
    g->addEdge(1, 3, 1, true, true);
    g->addEdge(2, 3, 2, true, false);
    g->addEdge(0, 3, 3, false, true);

    return g;
}

FlagsGraph* build_flags_graph_02() {
    FlagsGraph* g = new FlagsGraph(4);

    g->data(0).rank = 1;
    g->data(1).rank = 4;
    g->data(2).rank = 2;
    g->data(3).rank = 3;

    g->addEdge(0, 1, 668357, true, false);
    g->addEdge(0, 1, 672314, false, true);
    g->addEdge(3, 1, 99235, false, true);
    g->addEdge(2, 1, 866307, true, false);
    g->addEdge(2, 3, 331010, false, true);

    g->addEdge(3, 1, 1197317, true, false);

    return g;
}

FlagsGraph* build_flags_graph_02_div100() {
    FlagsGraph* g = new FlagsGraph(4);

    g->data(0).rank = 1;
    g->data(1).rank = 4;
    g->data(2).rank = 2;
    g->data(3).rank = 3;

    g->addEdge(0, 1, 6683, true, false);
    g->addEdge(0, 1, 6723, false, true);
    g->addEdge(3, 1, 992, false, true);
    g->addEdge(2, 1, 8663, true, false);
    g->addEdge(2, 3, 3310, false, true);

    g->addEdge(3, 1, 11973, true, false);

    return g;
}



TEST(ch_test, from_xengraph1) {
    run_preprocessor("ch xengraph functest/01_xengraph.xeng from_xengraph1");
    FlagsGraph* loaded = DDSGLoader("from_xengraph1.ch").loadFlagsGraph();
    FlagsGraph* expected = build_flags_graph_01();
    compare_flags_graphs(*loaded, *expected);
}

TEST(ch_test, from_xengraph2) {
    run_preprocessor("ch xengraph functest/02_xengraph.xeng from_xengraph2 100");
    FlagsGraph* loaded = DDSGLoader("from_xengraph2.ch").loadFlagsGraph();
    FlagsGraph* expected = build_flags_graph_02_div100();
    compare_flags_graphs(*loaded, *expected);
}



TEST(ch_test, from_csv1) {
    run_preprocessor("ch csv functest/02_adj.csv from_csv1");
    FlagsGraph* loaded = DDSGLoader("from_csv1.ch").loadFlagsGraph();
    FlagsGraph* expected = build_flags_graph_02();
    compare_flags_graphs(*loaded, *expected);
}

TEST(ch_test, from_csv2) {
    run_preprocessor("ch csv functest/02_adj.csv from_csv2 100");
    FlagsGraph* loaded = DDSGLoader("from_csv2.ch").loadFlagsGraph();
    FlagsGraph* expected = build_flags_graph_02_div100();
    compare_flags_graphs(*loaded, *expected);
}



TEST(ch_test, from_dimacs1) {
    run_preprocessor("ch dimacs functest/02_dimacs.gr from_dimacs1");
    FlagsGraph* loaded = DDSGLoader("from_dimacs1.ch").loadFlagsGraph();
    FlagsGraph* expected = build_flags_graph_02();
    compare_flags_graphs(*loaded, *expected);
}

TEST(ch_test, from_dimacs2) {
    run_preprocessor("ch dimacs functest/02_dimacs.gr from_dimacs2 100");
    FlagsGraph* loaded = DDSGLoader("from_dimacs2.ch").loadFlagsGraph();
    FlagsGraph* expected = build_flags_graph_02_div100();
    compare_flags_graphs(*loaded, *expected);
}
