//
// Created by horakj31 on 2024-03-12.
//
#include "gtest/gtest.h"
#include "common.h"

#include "GraphBuilding/Loaders/TNRGLoader.h"

TransitNodeRoutingGraph* build_tnr_graph_01_1() {
    TransitNodeRoutingGraph* g = new TransitNodeRoutingGraph(4, 1);

    g->addEdge(0, 1, 1, true, false);
    g->addEdge(0, 3, 3, false, true);
    g->addEdge(1, 3, 1, true, true);
    g->addEdge(2, 0, 2, false, true);
    g->addEdge(2, 3, 2, true, false);

    g->data(0).rank = 2;
    g->data(1).rank = 3;
    g->data(2).rank = 1;
    g->data(3).rank = 4;

    g->addMappingPair(3, 0);

    g->setDistanceTableValue(0, 0, 0);

    g->addForwardAccessNode(0, 3, 2);
    g->addBackwardAccessNode(0, 3, 3);
    g->addForwardAccessNode(1, 3, 1);
    g->addBackwardAccessNode(1, 3, 1);
    g->addForwardAccessNode(2, 3, 2);
    g->addBackwardAccessNode(2, 3, 5);
    g->addForwardAccessNode(3, 3, 0);
    g->addBackwardAccessNode(3, 3, 0);

    g->addForwardSearchSpaceNode(0, 0);
    g->addForwardSearchSpaceNode(0, 1);
    g->addBackwardSearchSpaceNode(0, 0);
    g->addForwardSearchSpaceNode(1, 1);
    g->addBackwardSearchSpaceNode(1, 1);
    g->addForwardSearchSpaceNode(2, 2);
    g->addBackwardSearchSpaceNode(2, 2);
    g->addBackwardSearchSpaceNode(2, 0);

    return g;
}

TransitNodeRoutingGraph* build_tnr_graph_01_2() {
    TransitNodeRoutingGraph* g = new TransitNodeRoutingGraph(4, 2);

    g->addEdge(0, 1, 1, true, false);
    g->addEdge(0, 3, 3, false, true);
    g->addEdge(1, 3, 1, true, true);
    g->addEdge(2, 0, 2, false, true);
    g->addEdge(2, 3, 2, true, false);

    g->data(0).rank = 2;
    g->data(1).rank = 3;
    g->data(2).rank = 1;
    g->data(3).rank = 4;

    g->addMappingPair(1, 0);
    g->addMappingPair(3, 1);

    g->setDistanceTableValue(0, 0, 0);
    g->setDistanceTableValue(0, 1, 1);
    g->setDistanceTableValue(1, 0, 1);
    g->setDistanceTableValue(1, 1, 0);

    g->addForwardAccessNode(0, 1, 1);
    g->addBackwardAccessNode(0, 3, 3);
    g->addForwardAccessNode(1, 1, 0);
    g->addBackwardAccessNode(1, 1, 0);
    g->addForwardAccessNode(2, 3, 2);
    g->addBackwardAccessNode(2, 3, 5);
    g->addForwardAccessNode(3, 3, 0);
    g->addBackwardAccessNode(3, 3, 0);

    g->addForwardSearchSpaceNode(0, 0);
    g->addBackwardSearchSpaceNode(0, 0);
    g->addForwardSearchSpaceNode(2, 2);
    g->addBackwardSearchSpaceNode(2, 2);
    g->addBackwardSearchSpaceNode(2, 0);

    return g;
}

TransitNodeRoutingGraph* build_tnr_graph_01_3() {
    TransitNodeRoutingGraph* g = new TransitNodeRoutingGraph(4, 3);

    g->addEdge(0, 1, 1, true, false);
    g->addEdge(0, 3, 3, false, true);
    g->addEdge(1, 3, 1, true, true);
    g->addEdge(2, 0, 2, false, true);
    g->addEdge(2, 3, 2, true, false);

    g->data(0).rank = 2;
    g->data(1).rank = 3;
    g->data(2).rank = 1;
    g->data(3).rank = 4;

    g->addMappingPair(0, 0);
    g->addMappingPair(1, 1);
    g->addMappingPair(3, 2);

    g->setDistanceTableValue(0, 0, 0);
    g->setDistanceTableValue(0, 1, 1);
    g->setDistanceTableValue(0, 2, 2);
    g->setDistanceTableValue(1, 0, 4);
    g->setDistanceTableValue(1, 1, 0);
    g->setDistanceTableValue(1, 2, 1);
    g->setDistanceTableValue(2, 0, 3);
    g->setDistanceTableValue(2, 1, 1);
    g->setDistanceTableValue(2, 2, 0);

    g->addForwardAccessNode(0, 0, 0);
    g->addBackwardAccessNode(0, 0, 0);
    g->addForwardAccessNode(1, 1, 0);
    g->addBackwardAccessNode(1, 1, 0);
    g->addForwardAccessNode(2, 3, 2);
    g->addBackwardAccessNode(2, 0, 2);
    g->addForwardAccessNode(3, 3, 0);
    g->addBackwardAccessNode(3, 3, 0);

    g->addForwardSearchSpaceNode(2, 2);
    g->addBackwardSearchSpaceNode(2, 2);

    return g;
}

TransitNodeRoutingGraph* build_tnr_graph_02_2_div100() {
    TransitNodeRoutingGraph* g = new TransitNodeRoutingGraph(4, 2);

    g->addEdge(0, 1, 6683, true, false);
    g->addEdge(0, 1, 6723, false, true);
    g->addEdge(2, 1, 8663, true, false);
    g->addEdge(2, 3, 3310, false, true);
    g->addEdge(3, 1, 992, false, true);
    g->addEdge(3, 1, 11973, true, false);

    g->data(0).rank = 1;
    g->data(1).rank = 4;
    g->data(2).rank = 2;
    g->data(3).rank = 3;

    g->addMappingPair(1, 0);
    g->addMappingPair(3, 1);

    g->setDistanceTableValue(0, 0, 0);
    g->setDistanceTableValue(0, 1, 992);
    g->setDistanceTableValue(1, 0, 11973);
    g->setDistanceTableValue(1, 1, 0);

    g->addForwardAccessNode(0, 1, 6683);
    g->addBackwardAccessNode(0, 1, 6723);
    g->addForwardAccessNode(1, 1, 0);
    g->addBackwardAccessNode(1, 1, 0);
    g->addForwardAccessNode(2, 1, 8663);
    g->addBackwardAccessNode(2, 3, 3310);
    g->addForwardAccessNode(3, 3, 0);
    g->addBackwardAccessNode(3, 3, 0);

    g->addForwardSearchSpaceNode(0, 0);
    g->addBackwardSearchSpaceNode(0, 0);
    g->addForwardSearchSpaceNode(2, 2);
    g->addBackwardSearchSpaceNode(2, 2);

    return g;
}

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
