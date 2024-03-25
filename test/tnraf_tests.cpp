//
// Created by david on 2023-07-22.
//

#include <filesystem>
#include "gtest/gtest.h"
#include "GraphBuilding/Structures/UpdateableGraph.h"
#include "GraphBuilding/Loaders/CsvGraphLoader.h"
#include "common.h"
#include "TNRAF/TNRAFPreprocessor.h"
#include "GraphBuilding/Loaders/TGAFLoader.h"

void compare_tnraf_graphs(
		const TransitNodeRoutingArcFlagsGraph& computed,
		const TransitNodeRoutingArcFlagsGraph& expected
) {
	compare_tnr_graphs(computed, expected);

	EXPECT_EQ(computed.getForwardAccessNodes(), expected.getForwardAccessNodes());
	EXPECT_EQ(computed.getBackwardAccessNodes(), expected.getBackwardAccessNodes());
	EXPECT_EQ(computed.getNodesData(), expected.getNodesData());
}

TEST(tnraf_test, simple_test) {
	std::filesystem::path adj_path("test/adj.csv");
	unsigned short region_count = 2;

	UpdateableGraph ch_graph(compute_ch_from_adj(std::filesystem::path("test/adj.csv")));
	CsvGraphLoader adj_loader(adj_path.string());
	Graph graph(adj_loader.nodes());
	adj_loader.loadGraph(graph);
	TNRAFPreprocessor::preprocessUsingCH(ch_graph, graph, "tmp", 1, region_count);
	TGAFLoader loader_slow("tmp.tgaf");
	auto* tnraf_graph_slow = loader_slow.loadTNRAFforDistanceQueries();

	UpdateableGraph ch_graph_2(compute_ch_from_adj(std::filesystem::path("test/adj.csv")));
	CsvGraphLoader adj_loader_2(adj_path.string());
	Graph graph_2(adj_loader_2.nodes());
	adj_loader_2.loadGraph(graph_2);
	TNRAFPreprocessor::preprocessUsingCH(ch_graph_2, graph_2, "tmp", 1, region_count, true);
	TGAFLoader loader_dm("tmp.tgaf");
	auto* tnraf_graph_dm = loader_dm.loadTNRAFforDistanceQueries();

	compare_tnraf_graphs(*tnraf_graph_slow, *tnraf_graph_dm);

	TGAFLoader loader_gt("test/test_1_gt.tgaf");
	auto* tnraf_graph_gt = loader_gt.loadTNRAFforDistanceQueries();

	compare_tnraf_graphs(*tnraf_graph_slow, *tnraf_graph_gt);
}
