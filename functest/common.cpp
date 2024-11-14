//
// Created by horakj31 on 2024-02-09.
//

#include "gtest/gtest.h"
#include <fstream>
#include <string>
#include <algorithm>
#include "common.h"


void run_preprocessor(const char* args) {
#ifdef _WIN32
    std::string command = std::string("shortestPathsPreprocessor create ") + args;
#else
    std::string command = std::string("./shortestPathsPreprocessor create ") + args;
#endif
    std::system(command.c_str());
}



void compare_tnraf_graphs(const TransitNodeRoutingArcFlagsGraph& computed, const TransitNodeRoutingArcFlagsGraph& expected) {
    compare_tnr_graphs(computed, expected);

    EXPECT_EQ(computed.getForwardAccessNodes(), expected.getForwardAccessNodes());
    EXPECT_EQ(computed.getBackwardAccessNodes(), expected.getBackwardAccessNodes());
    EXPECT_EQ(computed.getNodesData(), expected.getNodesData());
}
