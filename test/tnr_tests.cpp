//
// Created by david on 2023-07-22.
//

#include <filesystem>
#include "gtest/gtest.h"
#include "GraphBuilding/Loaders/CsvGraphLoader.h"
#include "GraphBuilding/Structures/UpdateableGraph.h"
#include "common.h"
#include "TNR/TNRPreprocessor.h"
#include "GraphBuilding/Loaders/TNRGLoader.h"


TEST(tnr_test, simple_test) {
    std::filesystem::path adj_path("test/adj.csv");
    UpdateableGraph ch_graph(compute_ch_from_adj(adj_path.string()));

    TNRPreprocessor::preprocessUsingCH(ch_graph, "tmp", 1);

    TNRGLoader tnrloader("tmp.tnrg");
    TransitNodeRoutingGraph* tnr_graph_fast = tnrloader.loadTNRforDistanceQueries();

    TNRGLoader tnrloader_gt_fast("test/test_1_gt_fast.tnrg");
    TransitNodeRoutingGraph* tnr_graph_gt_fast = tnrloader_gt_fast.loadTNRforDistanceQueries();

    compare_tnr_graphs(*tnr_graph_fast, *tnr_graph_gt_fast);

    // slow variant
    UpdateableGraph ch_graph_2(compute_ch_from_adj(std::filesystem::path("test/adj.csv")));
    CsvGraphLoader adj_loader(adj_path.string());
    Graph graph(adj_loader.nodes());
    adj_loader.loadGraph(graph);
    TNRPreprocessor::preprocessUsingCHslower(ch_graph_2, graph, "tmp", 1);
    TNRGLoader tnrloader_slow("tmp.tnrg");
    TransitNodeRoutingGraph* tnr_graph_slow = tnrloader.loadTNRforDistanceQueries();

    // dm
    UpdateableGraph ch_graph_3(compute_ch_from_adj(std::filesystem::path("test/adj.csv")));
    Graph graph_2(adj_loader.nodes());
    adj_loader.loadGraph(graph_2);
    TNRPreprocessor::preprocessWithDMvalidation(ch_graph_3, graph_2, "tmp", 1);
    TNRGLoader tnrloader_dm("tmp.tnrg");
    TransitNodeRoutingGraph* tnr_graph_dm = tnrloader.loadTNRforDistanceQueries();

    compare_tnr_graphs(*tnr_graph_slow, *tnr_graph_dm);

    TNRGLoader tnrloader_gt("test/test_1_gt.tnrg");
    TransitNodeRoutingGraph* tnr_graph_gt = tnrloader_gt_fast.loadTNRforDistanceQueries();

    compare_tnr_graphs(*tnr_graph_slow, *tnr_graph_gt);
}
