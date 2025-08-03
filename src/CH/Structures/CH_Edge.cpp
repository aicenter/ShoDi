//
// Created by david on 2025-08-02.
//

#include "CH_Edge.h"

node_id_type CH_Edge::get_to() {
	return to_node;
}

edge_weight_type CH_Edge::get_weight() {
	auto props = prop_array.to_ulong();
	constexpr auto mask = 0x3FFFFFFF;
	return props & mask;
}

bool CH_Edge::is_forward_edge() {
	return prop_array.test(30);
}

bool CH_Edge::is_backward_edge() {
	return prop_array.test(31);
}
