#include "expected_graphs.h"

FlagsGraph<NodeData>* build_flags_graph_01() {
    FlagsGraph<NodeData>* g = new FlagsGraph(4);

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

FlagsGraph<NodeData>* build_flags_graph_02() {
    FlagsGraph<NodeData>* g = new FlagsGraph(4);

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

FlagsGraph<NodeData>* build_flags_graph_02_div100() {
    FlagsGraph<NodeData>* g = new FlagsGraph(4);

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

TransitNodeRoutingGraph<NodeData>* build_tnr_graph_01_1() {
    TransitNodeRoutingGraph<NodeData>* g = new TransitNodeRoutingGraph(4, 1);

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

TransitNodeRoutingGraph<NodeData>* build_tnr_graph_01_2() {
    TransitNodeRoutingGraph<NodeData>* g = new TransitNodeRoutingGraph(4, 2);

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

TransitNodeRoutingGraph<NodeData>* build_tnr_graph_01_3() {
    TransitNodeRoutingGraph<NodeData>* g = new TransitNodeRoutingGraph(4, 3);

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

TransitNodeRoutingGraph<NodeData>* build_tnr_graph_02_2_div100() {
    TransitNodeRoutingGraph<NodeData>* g = new TransitNodeRoutingGraph(4, 2);

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
