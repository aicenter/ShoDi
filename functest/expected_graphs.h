/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

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
