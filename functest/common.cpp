//
// Created by horakj31 on 2024-02-09.
//

#include "gtest/gtest.h"
#include "common.h"
#include "GraphBuilding/Loaders/DDSGLoader.h"
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>

void run_preprocessor(const char* args) {
#ifdef _WIN32
    std::string command = std::string("shortestPathsPreprocessor create ") + args;
#else
    std::string command = std::string("./shortestPathsPreprocessor create ") + args;
#endif
    std::system(command.c_str());
}

void compare_files(const std::string& p1, const std::string& p2) {
    std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

    ASSERT_EQ(f1.fail(), false);
    ASSERT_EQ(f2.fail(), false);

    EXPECT_EQ(f1.tellg(), f2.tellg());

    //seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    EXPECT_EQ(
        std::equal(
            std::istreambuf_iterator<char>(f1.rdbuf()),
            std::istreambuf_iterator<char>(),
            std::istreambuf_iterator<char>(f2.rdbuf())
            ),
        true);
}

void compare_ch_files(const std::string& computed_fname, const std::string& expected_fname) {
    DDSGLoader chLoader1 = DDSGLoader(computed_fname);
    FlagsGraph* loaded = chLoader1.loadFlagsGraph();

    DDSGLoader chLoader2 = DDSGLoader(expected_fname);
    FlagsGraph* expected = chLoader2.loadFlagsGraph();

    compare_flags_graphs(*loaded, *expected);
}

void compare_flags_graphs(
    const FlagsGraph& computed,
    const FlagsGraph& expected
) {
    ASSERT_EQ(computed.nodes(), expected.nodes());

    const auto& computed_nodes_data = computed.getNodesData();
    const auto& expected_nodes_data = expected.getNodesData();

    for (unsigned int i = 0; i < computed_nodes_data.size(); ++i) {
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

    for (int i = 0; i < computed_neighbours.size(); i++) {
        ASSERT_EQ(computed_neighbours[i].size(), expected_neighbours[i].size());
        for (int j = 0; j < computed_neighbours[i].size(); j++) {
            ASSERT_EQ(computed_neighbours[i][j].targetNode, expected_neighbours[i][j].targetNode);
            ASSERT_EQ(computed_neighbours[i][j].weight, expected_neighbours[i][j].weight);
            ASSERT_EQ(computed_neighbours[i][j].forward, expected_neighbours[i][j].forward);
            ASSERT_EQ(computed_neighbours[i][j].backward, expected_neighbours[i][j].backward);
        }
    }
}
