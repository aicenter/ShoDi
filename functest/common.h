//
// Created by horakj31 on 2024-02-09.
//

#ifndef FUNCTEST_COMMON_H
#define FUNCTEST_COMMON_H

#include "gtest/gtest.h"
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include "common.h"

#include "GraphBuilding/Structures/FlagsGraph.h"
#include "GraphBuilding/Structures/TransitNodeRoutingGraph.h"
#include "GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.h"

std::string run_preprocessor(const std::string& args);

void compare_txt_files(const std::string& p1, const std::string& p2);

template<class T>
void compare_flags_graphs(
        const FlagsGraph<T>& computed,
        const FlagsGraph<T>& expected
) {
    ASSERT_EQ(computed.nodes(), expected.nodes());

    const auto& computed_nodes_data = computed.getNodesData();
    const auto& expected_nodes_data = expected.getNodesData();

    for (size_t i = 0; i < computed_nodes_data.size(); ++i) {
        ASSERT_EQ(computed_nodes_data[i].rank, expected_nodes_data[i].rank);
        ASSERT_EQ(computed_nodes_data[i].backwardDist, expected_nodes_data[i].backwardDist);
        ASSERT_EQ(computed_nodes_data[i].forwardDist, expected_nodes_data[i].forwardDist);
        ASSERT_EQ(computed_nodes_data[i].backwardReached, expected_nodes_data[i].backwardReached);
        ASSERT_EQ(computed_nodes_data[i].backwardSettled, expected_nodes_data[i].backwardSettled);
        ASSERT_EQ(computed_nodes_data[i].backwardStalled, expected_nodes_data[i].backwardStalled);
        ASSERT_EQ(computed_nodes_data[i].forwardReached, expected_nodes_data[i].forwardReached);
        ASSERT_EQ(computed_nodes_data[i].forwardSettled, expected_nodes_data[i].forwardSettled);
        ASSERT_EQ(computed_nodes_data[i].forwardStalled, expected_nodes_data[i].forwardStalled);
    }

    const auto& computed_neighbours = computed.getNeighbours();
    const auto& expected_neighbours = expected.getNeighbours();

    for (size_t i = 0; i < computed_neighbours.size(); i++) {
        ASSERT_EQ(computed_neighbours[i].size(), expected_neighbours[i].size());
        for (size_t j = 0; j < computed_neighbours[i].size(); j++) {
            ASSERT_EQ(computed_neighbours[i][j].targetNode, expected_neighbours[i][j].targetNode);
            ASSERT_EQ(computed_neighbours[i][j].weight, expected_neighbours[i][j].weight);
            ASSERT_EQ(computed_neighbours[i][j].forward, expected_neighbours[i][j].forward);
            ASSERT_EQ(computed_neighbours[i][j].backward, expected_neighbours[i][j].backward);
        }
    }
}

template<class T> void compare_tnr_graphs(
        const TransitNodeRoutingGraph<T>& computed,
        const TransitNodeRoutingGraph<T>& expected
) {
    compare_flags_graphs(computed, expected);

    EXPECT_EQ(computed.getBackwardAccessNodes(), expected.getBackwardAccessNodes());
    EXPECT_EQ(computed.getForwardAccessNodes(), expected.getForwardAccessNodes());
    EXPECT_EQ(computed.getBackwardSearchSpaces(), expected.getBackwardSearchSpaces());
    EXPECT_EQ(computed.getForwardSearchSpaces(), expected.getForwardSearchSpaces());
}

void compare_tnraf_graphs(
        const TransitNodeRoutingArcFlagsGraph& computed,
        const TransitNodeRoutingArcFlagsGraph& expected
);

#endif
