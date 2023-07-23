//
// Created by david on 2023-07-22.
//

#pragma once

#include <filesystem>
#include "GraphBuilding/Loaders/CsvGraphLoader.h"
#include "GraphBuilding/Structures/UpdateableGraph.h"
#include "CH/CHPreprocessor.h"
#include "GraphBuilding/Structures/FlagsGraph.h"
#include "GraphBuilding/Structures/TransitNodeRoutingGraph.h"

UpdateableGraph compute_ch_from_adj(const std::filesystem::path& adj_path);

void compare_flags_graphs(const FlagsGraph& computed, const FlagsGraph& expected);

void compare_tnr_graphs(const TransitNodeRoutingGraph& computed, const TransitNodeRoutingGraph& expected);
