//
// Created by horakj31 on 2024-02-09.
//

#include "gtest/gtest.h"
#include "common.h"
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>

void run_preprocessor(const char* args) {
    std::string command = std::string("shortestPathsPreprocessor create ") + args;
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
