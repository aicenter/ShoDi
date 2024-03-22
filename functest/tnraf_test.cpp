//
// Created by horakj31 on 2024-03-17.
//
#include "gtest/gtest.h"
#include "common.h"

#include "GraphBuilding/Loaders/TGAFLoader.h"
#include "TNRAF/TNRAFPreprocessor.h"

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_01_1() {
    // tnodes_cnt = 1
    TransitNodeRoutingArcFlagsGraph* g = new TransitNodeRoutingArcFlagsGraph(4, 1);

    g->addEdge(0, 1, 1, true, false);
    g->addEdge(0, 3, 3, false, true);
    g->addEdge(1, 3, 1, true, true);
    g->addEdge(2, 0, 2, false, true);
    g->addEdge(2, 3, 2, true, false);

    g->data(0).rank = 2;
    g->data(1).rank = 3;
    g->data(2).rank = 1;
    g->data(3).rank = 4;

    g->data(0).region = 0;
    g->data(1).region = 1;
    g->data(2).region = 2;
    g->data(3).region = 3;

    g->addMappingPair(3, 0);

    g->setDistanceTableValue(0, 0, 0);

    std::vector<unsigned int> powersOf2(4);
    TNRAFPreprocessor::getPowersOf2(powersOf2);
    g->addForwardAccessNode(0, 3, 2, 4, 8, powersOf2);
    g->addBackwardAccessNode(0, 3, 3, 4, 14, powersOf2);
    g->addForwardAccessNode(1, 3, 1, 4, 13, powersOf2);
    g->addBackwardAccessNode(1, 3, 1, 4, 12, powersOf2);
    g->addForwardAccessNode(2, 3, 2, 4, 11, powersOf2);
    g->addBackwardAccessNode(2, 3, 5, 4, 10, powersOf2);
    g->addForwardAccessNode(3, 3, 0, 4, 15, powersOf2);
    g->addBackwardAccessNode(3, 3, 0, 4, 15, powersOf2);

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

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_01_2() {
    // tnodes_cnt = 2
    TransitNodeRoutingArcFlagsGraph* g = new TransitNodeRoutingArcFlagsGraph(4, 2);

    g->addEdge(0, 1, 1, true, false);
    g->addEdge(0, 3, 3, false, true);
    g->addEdge(1, 3, 1, true, true);
    g->addEdge(2, 0, 2, false, true);
    g->addEdge(2, 3, 2, true, false);

    g->data(0).rank = 2;
    g->data(1).rank = 3;
    g->data(2).rank = 1;
    g->data(3).rank = 4;

    g->data(0).region = 0;
    g->data(1).region = 1;
    g->data(2).region = 2;
    g->data(3).region = 3;

    g->addMappingPair(1, 0);
    g->addMappingPair(3, 1);

    g->setDistanceTableValue(0, 0, 0);
    g->setDistanceTableValue(0, 1, 1);
    g->setDistanceTableValue(1, 0, 1);
    g->setDistanceTableValue(1, 1, 0);

    std::vector<unsigned int> powersOf2(4);
    TNRAFPreprocessor::getPowersOf2(powersOf2);
    g->addForwardAccessNode(0, 1, 1, 4, 10, powersOf2);
    g->addBackwardAccessNode(0, 3, 3, 4, 14, powersOf2);
    g->addForwardAccessNode(1, 1, 0, 4, 15, powersOf2);
    g->addBackwardAccessNode(1, 1, 0, 4, 15, powersOf2);
    g->addForwardAccessNode(2, 3, 2, 4, 11, powersOf2);
    g->addBackwardAccessNode(2, 3, 5, 4, 10, powersOf2);
    g->addForwardAccessNode(3, 3, 0, 4, 15, powersOf2);
    g->addBackwardAccessNode(3, 3, 0, 4, 15, powersOf2);

    g->addForwardSearchSpaceNode(0, 0);
    g->addBackwardSearchSpaceNode(0, 0);
    g->addForwardSearchSpaceNode(2, 2);
    g->addBackwardSearchSpaceNode(2, 2);
    g->addBackwardSearchSpaceNode(2, 0);

    return g;
}

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_01_3() {
    // tnodes_cnt = 3
    TransitNodeRoutingArcFlagsGraph* g = new TransitNodeRoutingArcFlagsGraph(4, 3);

    g->addEdge(0, 1, 1, true, false);
    g->addEdge(0, 3, 3, false, true);
    g->addEdge(1, 3, 1, true, true);
    g->addEdge(2, 0, 2, false, true);
    g->addEdge(2, 3, 2, true, false);

    g->data(0).rank = 2;
    g->data(1).rank = 3;
    g->data(2).rank = 1;
    g->data(3).rank = 4;

    g->data(0).region = 0;
    g->data(1).region = 1;
    g->data(2).region = 2;
    g->data(3).region = 3;

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

    std::vector<unsigned int> powersOf2(4);
    TNRAFPreprocessor::getPowersOf2(powersOf2);
    g->addForwardAccessNode(0, 0, 0, 4, 15, powersOf2);
    g->addBackwardAccessNode(0, 0, 0, 4, 15, powersOf2);
    g->addForwardAccessNode(1, 1, 0, 4, 15, powersOf2);
    g->addBackwardAccessNode(1, 1, 0, 4, 15, powersOf2);
    g->addForwardAccessNode(2, 3, 2, 4, 11, powersOf2);
    g->addBackwardAccessNode(2, 0, 2, 4, 11, powersOf2);
    g->addForwardAccessNode(3, 3, 0, 4, 15, powersOf2);
    g->addBackwardAccessNode(3, 3, 0, 4, 15, powersOf2);

    g->addForwardSearchSpaceNode(2, 2);
    g->addBackwardSearchSpaceNode(2, 2);

    return g;
}

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_02_3_div100() {
    // tnodes_cnt = 3
    TransitNodeRoutingArcFlagsGraph* g = new TransitNodeRoutingArcFlagsGraph(4, 3);

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

    g->data(0).region = 0;
    g->data(1).region = 1;
    g->data(2).region = 2;
    g->data(3).region = 3;

    g->addMappingPair(1, 0);
    g->addMappingPair(2, 1);
    g->addMappingPair(3, 2);

    g->setDistanceTableValue(0, 0, 0);
    g->setDistanceTableValue(0, 1, 4302);
    g->setDistanceTableValue(0, 2, 992);
    g->setDistanceTableValue(1, 0, 8663);
    g->setDistanceTableValue(1, 1, 0);
    g->setDistanceTableValue(1, 2, 9655);
    g->setDistanceTableValue(2, 0, 11973);
    g->setDistanceTableValue(2, 1, 3310);
    g->setDistanceTableValue(2, 2, 0);

    std::vector<unsigned int> powersOf2(4);
    TNRAFPreprocessor::getPowersOf2(powersOf2);
    g->addForwardAccessNode(0, 1, 6683, 4, 14, powersOf2);
    g->addBackwardAccessNode(0, 1, 6723, 4, 14, powersOf2);
    g->addForwardAccessNode(1, 1, 0, 4, 15, powersOf2);
    g->addBackwardAccessNode(1, 1, 0, 4, 15, powersOf2);
    g->addForwardAccessNode(2, 2, 0, 4, 15, powersOf2);
    g->addBackwardAccessNode(2, 2, 0, 4, 15, powersOf2);
    g->addForwardAccessNode(3, 3, 0, 4, 15, powersOf2);
    g->addBackwardAccessNode(3, 3, 0, 4, 15, powersOf2);

    g->addForwardSearchSpaceNode(0, 0);
    g->addBackwardSearchSpaceNode(0, 0);

    return g;
}

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
