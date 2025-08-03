//
// Created by david on 2025-08-02.
//

#pragma once

#include <bitset>

#include "aliases.h"
#include "../../aliases.h"

class CH_Edge {
public:
	node_id_type get_to();

	edge_weight_type get_weight();

	bool is_forward_edge();

	bool is_backward_edge();

private:
	node_id_type to_node;
	std::bitset<32> prop_array;
};
