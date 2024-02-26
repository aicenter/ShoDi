//
// Created by horakj31 on 2024-02-21.
//
#include <fstream>
#include "gtest/gtest.h"
#include "common.h"
#include "API/CHDistanceQueryManagerAPI.h"

TEST(ch_test, xengraph_to_ch1) {
    run_preprocessor("ch xengraph functest/01_xengraph.xeng xengraph_to_ch1");

    auto api = CHDistanceQueryManagerAPI();
    api.initializeCH("xengraph_to_ch1.ch", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 1), 1);
    EXPECT_EQ(api.distanceQuery(1, 0), 4);
    EXPECT_EQ(api.distanceQuery(1, 2), 6);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(ch_test, xengraph_to_ch2) {
    run_preprocessor("ch xengraph functest/02_xengraph.xeng xengraph_to_ch2 2");

    auto api = CHDistanceQueryManagerAPI();
    api.initializeCH("xengraph_to_ch2.ch", "functest/02_mapping.txt");

    EXPECT_EQ(api.distanceQuery(55, 66), 5);
    EXPECT_EQ(api.distanceQuery(77, 88), 11);
    EXPECT_EQ(api.distanceQuery(66, 55), 15);

    api.clearStructures();
}

TEST(ch_test, dimacs_to_ch1) {
    run_preprocessor("ch dimacs functest/01_dimacs.gr dimacs_to_ch1");

    auto api = CHDistanceQueryManagerAPI();
    api.initializeCH("dimacs_to_ch1.ch", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(2, 1), 3);
    EXPECT_EQ(api.distanceQuery(1, 3), 1);
    EXPECT_EQ(api.distanceQuery(0, 3), 2);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(ch_test, dimacs_to_ch2) {
    run_preprocessor("ch dimacs functest/02_dimacs.gr dimacs_to_ch2 2");

    auto api = CHDistanceQueryManagerAPI();
    api.initializeCH("dimacs_to_ch2.ch", "functest/02_mapping.txt");

    EXPECT_EQ(api.distanceQuery(55, 66), 5);
    EXPECT_EQ(api.distanceQuery(77, 88), 11);
    EXPECT_EQ(api.distanceQuery(66, 55), 15);
    EXPECT_EQ(api.distanceQuery(77, 77), 0);

    api.clearStructures();
}

TEST(ch_test, csv_to_ch1) {
    run_preprocessor("ch csv functest/01_adj.csv csv_to_ch1");

    auto api = CHDistanceQueryManagerAPI();
    api.initializeCH("csv_to_ch1.ch", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(3, 0), 3);
    EXPECT_EQ(api.distanceQuery(2, 3), 2);
    EXPECT_EQ(api.distanceQuery(2, 0), 5);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(ch_test, csv_to_ch2) {
    run_preprocessor("ch csv functest/02_adj.csv csv_to_ch2 2");

    auto api = CHDistanceQueryManagerAPI();
    api.initializeCH("csv_to_ch2.ch", "functest/02_mapping.txt");

    EXPECT_EQ(api.distanceQuery(66, 66), 0);
    EXPECT_EQ(api.distanceQuery(77, 66), 10);
    EXPECT_EQ(api.distanceQuery(66, 88), 21);

    api.clearStructures();
}
