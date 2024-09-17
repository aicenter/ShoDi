//
// Created by horakj31 on 2024-02-09.
//

#include "gtest/gtest.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <array>
#include "common.h"


std::string run_preprocessor(const std::string& args) {
#ifdef _WIN32
    std::string command = "shortestPathsPreprocessor create " + args + " 2>&1";
#else
    std::string command = "./shortestPathsPreprocessor create " + args + " 2>&1";
#endif

    std::array<char, 128> buffer{};
    std::string result;

#ifdef _WIN32
    const auto pipe = _popen(command.c_str(), "r");
#else
    const auto pipe = popen(command.c_str(), "r");
#endif
    if (!pipe) throw std::runtime_error("Cannot run preprocessor: popen() failed!");

    while (!feof(pipe)) {
        if (fgets(buffer.data(), 128, pipe) != nullptr)
            result += buffer.data();
    }
    

#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
    return result;
}

void compare_txt_files(
        const std::string& p1,
        const std::string& p2
) {
    std::ifstream f1(p1);
    std::ifstream f2(p2);

    ASSERT_EQ(f1.is_open(), true);
    ASSERT_EQ(f2.is_open(), true);

    std::string line1, line2;
    while (std::getline(f1, line1) && std::getline(f2, line2)) {
        EXPECT_EQ(line1, line2);
    }
}

void compare_tnraf_graphs(
        const TransitNodeRoutingArcFlagsGraph& computed,
        const TransitNodeRoutingArcFlagsGraph& expected
) {
    compare_tnr_graphs(computed, expected);

    EXPECT_EQ(computed.getForwardAccessNodes(), expected.getForwardAccessNodes());
    EXPECT_EQ(computed.getBackwardAccessNodes(), expected.getBackwardAccessNodes());
    EXPECT_EQ(computed.getNodesData(), expected.getNodesData());
}
