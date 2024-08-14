//
// Created by horakj31 on 2024-03-25.
//

#pragma once

#include "GraphBuilding/Structures/FlagsGraph.h"
#include "GraphBuilding/Structures/TransitNodeRoutingGraph.h"
#include "GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.h"
#include "TNRAF/TNRAFPreprocessor.h"

FlagsGraph<NodeData>* build_flags_graph_01();

FlagsGraph<NodeData>* build_flags_graph_02();

FlagsGraph<NodeData>* build_flags_graph_02_div100();

TransitNodeRoutingGraph<NodeData>* build_tnr_graph_01_1();

TransitNodeRoutingGraph<NodeData>* build_tnr_graph_01_2();

TransitNodeRoutingGraph<NodeData>* build_tnr_graph_01_3();

TransitNodeRoutingGraph<NodeData>* build_tnr_graph_02_2_div100();

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_01_1();

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_01_2();

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_01_3();

TransitNodeRoutingArcFlagsGraph* build_tnraf_graph_02_3_div100();
