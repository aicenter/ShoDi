//
// Created by horakj31 on 2024-03-25.
//

#pragma once

#include "GraphBuilding/Structures/FlagsGraph.h"
#include "GraphBuilding/Structures/TransitNodeRoutingGraph.h"
#include "GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.h"
#include "TNRAF/TNRAFPreprocessor.h"

FlagsGraph* build_flags_graph_01();

FlagsGraph* build_flags_graph_02();

FlagsGraph* build_flags_graph_02_div100();

TransitNodeRoutingGraph* build_tnr_graph_01_1();

TransitNodeRoutingGraph* build_tnr_graph_01_2();

TransitNodeRoutingGraph* build_tnr_graph_01_3();

TransitNodeRoutingGraph* build_tnr_graph_02_2_div100();

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_01_1();

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_01_2();

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_01_3();

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_02_3_div100();
