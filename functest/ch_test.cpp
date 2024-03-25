//
// Created by horakj31 on 2024-03-01.
//
#include "gtest/gtest.h"
#include "common.h"
#include "expected_graphs.h"

#include "GraphBuilding/Loaders/DDSGLoader.h"


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
