/*
 * Edge.cpp
 *
 *  Created on: 3 dec 2010
 *      Author: samuel
 */

#include "Edge.h"
#include <assert.h>

Edge::Edge() : cost(1)
{
}

Edge::~Edge()
{
}

int Edge::get_cost()
{
	/// Returns cost of edge
	return cost;
}

void Edge::add_node(Node* new_node)
{
	my_nodes.push_back(new_node);
	assert(my_nodes.size() <= 2);
}
