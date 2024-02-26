//
// Created by horakj31 on 2024-02-21.
//
#include <fstream>
#include "gtest/gtest.h"
#include "common.h"
#include "API/TNRAFDistanceQueryManagerAPI.h"

TEST(tnraf_test, xengraph_to_tnraf1) {
    run_preprocessor("tnraf xengraph slow 3 functest/01_xengraph.xeng xengraph_to_tnraf1");

    auto api = TNRAFDistanceQueryManagerAPI();
    api.initializeTNRAF("xengraph_to_tnraf1.tgaf", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(2, 1), 3);
    EXPECT_EQ(api.distanceQuery(1, 3), 1);
    EXPECT_EQ(api.distanceQuery(0, 3), 2);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnraf_test, xengraph_to_tnraf2) {
    run_preprocessor("tnraf xengraph dm 2 functest/01_xengraph.xeng xengraph_to_tnraf2");

    auto api = TNRAFDistanceQueryManagerAPI();
    api.initializeTNRAF("xengraph_to_tnraf2.tgaf", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(3, 0), 3);
    EXPECT_EQ(api.distanceQuery(2, 3), 2);
    EXPECT_EQ(api.distanceQuery(2, 0), 5);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnraf_test, xengraph_to_tnraf3) {
    run_preprocessor("tnraf xengraph dm 3 functest/02_xengraph.xeng xengraph_to_tnraf3 2");

    auto api = TNRAFDistanceQueryManagerAPI();
    api.initializeTNRAF("xengraph_to_tnraf3.tgaf", "functest/02_mapping.txt");

    EXPECT_EQ(api.distanceQuery(77, 66), 10);
    EXPECT_EQ(api.distanceQuery(88, 88), 0);
    EXPECT_EQ(api.distanceQuery(88, 55), 5);

    api.clearStructures();
}

TEST(tnraf_test, dimacs_to_tnraf1) {
    run_preprocessor("tnraf dimacs slow 3 functest/01_dimacs.gr dimacs_to_tnraf1");

    auto api = TNRAFDistanceQueryManagerAPI();
    api.initializeTNRAF("dimacs_to_tnraf1.tgaf", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 1), 1);
    EXPECT_EQ(api.distanceQuery(2, 1), 3);
    EXPECT_EQ(api.distanceQuery(1, 3), 1);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnraf_test, dimacs_to_tnraf2) {
    run_preprocessor("tnraf dimacs dm 2 functest/01_dimacs.gr dimacs_to_tnraf2");

    auto api = TNRAFDistanceQueryManagerAPI();
    api.initializeTNRAF("dimacs_to_tnraf2.tgaf", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 2), 2);
    EXPECT_EQ(api.distanceQuery(1, 0), 4);
    EXPECT_EQ(api.distanceQuery(0, 3), 2);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnraf_test, dimacs_to_tnraf4) {
    run_preprocessor("tnraf dimacs dm 2 functest/02_dimacs.gr dimacs_to_tnraf4 2");

    auto api = TNRAFDistanceQueryManagerAPI();
    api.initializeTNRAF("dimacs_to_tnraf4.tgaf", "functest/02_mapping.txt");

    EXPECT_EQ(api.distanceQuery(55, 77), 15);
    EXPECT_EQ(api.distanceQuery(55, 55), 0);
    EXPECT_EQ(api.distanceQuery(77, 66), 10);

    api.clearStructures();
}

TEST(tnraf_test, csv_to_tnraf1) {
    run_preprocessor("tnraf csv slow 2 functest/01_adj.csv csv_to_tnraf1");

    auto api = TNRAFDistanceQueryManagerAPI();
    api.initializeTNRAF("csv_to_tnraf1.tgaf", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 2), 2);
    EXPECT_EQ(api.distanceQuery(1, 0), 4);
    EXPECT_EQ(api.distanceQuery(2, 0), 5);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnraf_test, csv_to_tnraf2) {
    run_preprocessor("tnraf csv dm 1 functest/01_adj.csv csv_to_tnraf2");

    auto api = TNRAFDistanceQueryManagerAPI();
    api.initializeTNRAF("csv_to_tnraf2.tgaf", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 2), 2);
    EXPECT_EQ(api.distanceQuery(1, 0), 4);
    EXPECT_EQ(api.distanceQuery(3, 0), 3);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnraf_test, csv_to_tnraf3) {
    run_preprocessor("tnraf csv dm 1 functest/02_adj.csv csv_to_tnraf3 2");

    auto api = TNRAFDistanceQueryManagerAPI();
    api.initializeTNRAF("csv_to_tnraf3.tgaf", "functest/02_mapping.txt");

    EXPECT_EQ(api.distanceQuery(66, 88), 21);
    EXPECT_EQ(api.distanceQuery(66, 77), 10);
    EXPECT_EQ(api.distanceQuery(66, 66), 0);
    EXPECT_EQ(api.distanceQuery(66, 55), 15);

    api.clearStructures();
}
