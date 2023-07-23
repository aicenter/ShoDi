//
// Created by david on 2023-07-21.
//

#include "gtest/gtest.h"
#include "GraphBuilding/Loaders/CsvGraphLoader.h"
#include "CH/CHPreprocessor.h"
#include "GraphBuilding/Loaders/DDSGLoader.h"
#include "common.h"



TEST(ch_test, simple_test) {
    UpdateableGraph graph(compute_ch_from_adj(std::filesystem::path("../data/test/adj.csv")));

    FlagsGraph flags_graph(graph);

    DDSGLoader chLoader = DDSGLoader("../data/test/test_1_gt.ch");
    FlagsGraph* gt_graph = chLoader.loadFlagsGraph();

    compare_flags_graphs(flags_graph, *gt_graph);
}
