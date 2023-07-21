//
// Created by david on 2023-07-19.
//
#include "gtest/gtest.h"
#include "../src/GraphBuilding/Loaders/CsvGraphLoader.h"
#include "../src/GraphBuilding/Structures/AdjMatrixGraph.h"
#include "DistanceMatrix/DistanceMatrixComputorFast.h"
#include "DistanceMatrix/DistanceMatrixComputorSlow.h"
#include "../src/DistanceMatrix/CSV_reader.h"

void compare_dist_matrices(
    const Distance_matrix_travel_time_provider& computed,
    const Distance_matrix_travel_time_provider& expected
) {
    ASSERT_EQ(computed.nodes(), expected.nodes());
    for (unsigned int i = 0; i < computed.nodes(); ++i) {
        for (unsigned int j = 0; j < computed.nodes(); ++j) {
            EXPECT_EQ(computed.findDistance(i, j), expected.findDistance(i, j));
        }
    }
}

TEST(dm_test, simple_test) {
    CsvGraphLoader adj_loader("../data/test/adj.csv");

    DistanceMatrixComputorFast dm_computor;
    auto matrix = dm_computor.compute_and_get_distance_matrix(adj_loader);

    DistanceMatrixComputorSlow dm_computor_slow;
    auto matrix_slow = dm_computor_slow.compute_and_get_distance_matrix(adj_loader);

    auto expected_matrix(
            std::make_from_tuple<Distance_matrix_travel_time_provider>(CSV_reader().read_matrix("../data/test/dm.csv")));

    compare_dist_matrices(*matrix, *matrix_slow);
    compare_dist_matrices(*matrix, expected_matrix);
}
