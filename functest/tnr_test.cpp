//
// Created by horakj31 on 2024-02-21.
//
#include <fstream>
#include "gtest/gtest.h"
#include "common.h"
#include "API/TNRDistanceQueryManagerAPI.h"

TEST(tnr_test, xengraph_to_tnr1) {
    run_preprocessor("tnr xengraph slow 1 functest/01_xengraph.xeng xengraph_to_tnr1");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("xengraph_to_tnr1.tnrg", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 1), 1);
    EXPECT_EQ(api.distanceQuery(1, 0), 4);
    EXPECT_EQ(api.distanceQuery(1, 2), 6);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnr_test, xengraph_to_tnr2) {
    run_preprocessor("tnr xengraph fast 2 functest/01_xengraph.xeng xengraph_to_tnr2");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("xengraph_to_tnr2.tnrg", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 1), 1);
    EXPECT_EQ(api.distanceQuery(1, 0), 4);
    EXPECT_EQ(api.distanceQuery(1, 2), 6);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnr_test, xengraph_to_tnr3) {
    run_preprocessor("tnr xengraph dm 3 functest/01_xengraph.xeng xengraph_to_tnr3");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("xengraph_to_tnr3.tnrg", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 1), 1);
    EXPECT_EQ(api.distanceQuery(1, 0), 4);
    EXPECT_EQ(api.distanceQuery(1, 2), 6);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnr_test, xengraph_to_tnr4) {
    run_preprocessor("tnr xengraph slow 3 functest/02_xengraph.xeng xengraph_to_tnr4 2");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("xengraph_to_tnr4.tnrg", "functest/02_mapping.txt");

    EXPECT_EQ(api.distanceQuery(77, 66), 10);
    EXPECT_EQ(api.distanceQuery(88, 88), 0);
    EXPECT_EQ(api.distanceQuery(88, 55), 5);

    api.clearStructures();
}

TEST(tnr_test, dimacs_to_tnr1) {
    run_preprocessor("tnr dimacs slow 3 functest/01_dimacs.gr dimacs_to_tnr1");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("dimacs_to_tnr1.tnrg", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 1), 1);
    EXPECT_EQ(api.distanceQuery(1, 0), 4);
    EXPECT_EQ(api.distanceQuery(1, 2), 6);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnr_test, dimacs_to_tnr2) {
    run_preprocessor("tnr dimacs fast 1 functest/01_dimacs.gr dimacs_to_tnr2");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("dimacs_to_tnr2.tnrg", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(3, 0), 3);
    EXPECT_EQ(api.distanceQuery(2, 3), 2);
    EXPECT_EQ(api.distanceQuery(2, 0), 5);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnr_test, dimacs_to_tnr3) {
    run_preprocessor("tnr dimacs dm 2 functest/01_dimacs.gr dimacs_to_tnr3");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("dimacs_to_tnr3.tnrg", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 1), 1);
    EXPECT_EQ(api.distanceQuery(1, 0), 4);
    EXPECT_EQ(api.distanceQuery(1, 2), 6);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnr_test, dimacs_to_tnr4) {
    run_preprocessor("tnr dimacs fast 2 functest/02_dimacs.gr dimacs_to_tnr4 2");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("dimacs_to_tnr4.tnrg", "functest/02_mapping.txt");

    EXPECT_EQ(api.distanceQuery(55, 77), 15);
    EXPECT_EQ(api.distanceQuery(55, 55), 0);
    EXPECT_EQ(api.distanceQuery(77, 66), 10);

    api.clearStructures();
}

TEST(tnr_test, csv_to_tnr1) {
    run_preprocessor("tnr csv slow 2 functest/01_adj.csv csv_to_tnr1");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("csv_to_tnr1.tnrg", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(1, 3), 1);
    EXPECT_EQ(api.distanceQuery(0, 2), 2);
    EXPECT_EQ(api.distanceQuery(0, 1), 1);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnr_test, csv_to_tnr2) {
    run_preprocessor("tnr csv fast 3 functest/01_adj.csv csv_to_tnr2");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("csv_to_tnr2.tnrg", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(0, 1), 1);
    EXPECT_EQ(api.distanceQuery(1, 0), 4);
    EXPECT_EQ(api.distanceQuery(1, 2), 6);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnr_test, csv_to_tnr3) {
    run_preprocessor("tnr csv dm 1 functest/01_adj.csv csv_to_tnr3");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("csv_to_tnr3.tnrg", "functest/01_mapping.txt");

    EXPECT_EQ(api.distanceQuery(2, 1), 3);
    EXPECT_EQ(api.distanceQuery(1, 3), 1);
    EXPECT_EQ(api.distanceQuery(0, 3), 2);

    for (unsigned i = 0; i < 4; i++) {
        EXPECT_EQ(api.distanceQuery(i, i), 0);
    }

    api.clearStructures();
}

TEST(tnr_test, csv_to_tnr4) {
    run_preprocessor("tnr csv dm 1 functest/02_adj.csv csv_to_tnr4 2");

    auto api = TNRDistanceQueryManagerAPI();
    api.initializeTNR("csv_to_tnr4.tnrg", "functest/02_mapping.txt");

    EXPECT_EQ(api.distanceQuery(66, 88), 21);
    EXPECT_EQ(api.distanceQuery(66, 77), 10);
    EXPECT_EQ(api.distanceQuery(66, 66), 0);
    EXPECT_EQ(api.distanceQuery(66, 55), 15);

    api.clearStructures();
}
