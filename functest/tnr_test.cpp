//
// Created by horakj31 on 2024-03-12.
//
#include "gtest/gtest.h"
#include "common.h"
#include "expected_graphs.h"

#include "GraphBuilding/Loaders/TNRGLoader.h"

TEST(tnr_test, from_xengraph1) {
    run_preprocessor("--method tnr --input-format xengraph --preprocessing-mode fast --tnodes-cnt 1 --input-file functest/01_xengraph.xeng --output-file from_xengraph1");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_xengraph1.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_1();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_xengraph2) {
    run_preprocessor("--method tnr --input-format xengraph --preprocessing-mode slow --tnodes-cnt 2 --input-file functest/01_xengraph.xeng --output-file from_xengraph2");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_xengraph2.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_2();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_xengraph3) {
    run_preprocessor("--method tnr --preprocessing-mode dm --tnodes-cnt 3 --input-file functest/01_xengraph.xeng --output-file from_xengraph3");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_xengraph3.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_3();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_xengraph4) {
    run_preprocessor("--method tnr --input-format xengraph --preprocessing-mode fast --tnodes-cnt 2 --input-file functest/02_xengraph.xeng --output-file from_xengraph4 --precision-loss 100");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_xengraph4.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_02_2_div100();
    compare_tnr_graphs(*loaded, *expected);
}



TEST(tnr_test, from_dimacs1) {
    run_preprocessor("--method tnr --input-format dimacs --preprocessing-mode fast --tnodes-cnt 2 --input-file functest/01_dimacs.gr --output-file from_dimacs1");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_dimacs1.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_2();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_dimacs2) {
    run_preprocessor("--method tnr --input-format dimacs --preprocessing-mode slow --tnodes-cnt 3 --input-file functest/01_dimacs.gr --output-file from_dimacs2");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_dimacs2.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_3();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_dimacs3) {
    run_preprocessor("--method tnr --preprocessing-mode dm --tnodes-cnt 1 --input-file functest/01_dimacs.gr --output-file from_dimacs3");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_dimacs3.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_1();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_dimacs4) {
    run_preprocessor("--method tnr --input-format dimacs --preprocessing-mode slow --tnodes-cnt 2 --input-file functest/02_dimacs.gr --output-file from_dimacs4 --precision-loss 100");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_dimacs4.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_02_2_div100();
    compare_tnr_graphs(*loaded, *expected);
}



TEST(tnr_test, from_adj1) {
    run_preprocessor("--method tnr --input-format adj --preprocessing-mode fast --tnodes-cnt 3 --input-file functest/01_adj.csv --output-file from_adj1");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_adj1.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_3();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_adj2) {
    run_preprocessor("--method tnr --input-format adj --preprocessing-mode slow --tnodes-cnt 1 --input-file functest/01_adj.csv --output-file from_adj2");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_adj2.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_1();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_adj3) {
    run_preprocessor("-m tnr --input-format adj --preprocessing-mode dm --tnodes-cnt 2 --input-file functest/01_adj.csv --output-file from_adj3");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_adj3.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_01_2();
    compare_tnr_graphs(*loaded, *expected);
}

TEST(tnr_test, from_adj4) {
    run_preprocessor("--method tnr --input-format adj --preprocessing-mode dm --tnodes-cnt 2 --input-file functest/02_adj.csv --output-file from_adj4 --precision-loss 100");
    TransitNodeRoutingGraph* loaded = TNRGLoader("from_adj4.tnrg").loadTNRforDistanceQueries();
    TransitNodeRoutingGraph* expected = build_tnr_graph_02_2_div100();
    compare_tnr_graphs(*loaded, *expected);
}
