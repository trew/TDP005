/*
 * Node.cpp
 *
 *  Created on: 3 dec 2010
 *      Author: samuel
 */

#include "Node.h"
#include <cstddef>

const int max_cost = 1000000;

Node::Node() :
		parent(NULL), cost_from_start(max_cost), allowed(true) {
}

Node::~Node() {
	///Destructor
}

void Node::set_allowed(bool _allowed) {
	///Set node to be or not to be allowed
	allowed = _allowed;
}

bool Node::is_allowed() {
	///Is node allowed to walk on?
	return allowed;
}

Node* Node::get_parent() {
	///Returns parent of node. Only returns good result if the Dijkstra algorithm has been run.
	return parent;
}

void Node::set_parent(Node* _parent) {
	///Set new parent
	parent = _parent;
}

int Node::get_cost_from_start() {
	///Returns cost from start
	return cost_from_start;
}

void Node::set_cost_from_start(int cost) {
	///Set new cost from start
	cost_from_start = cost;
}

NodeVector Node::get_neighbors() {
	return neighbors;
}

void Node::add_neighbor(Node* neighbor) {
	/**
	 * Adds the neighbor to the list of neighbors if it has not
	 * been added before.
	 */
	if (neighbor != NULL) {
		for (NodeVector::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
			if ((*it) == neighbor)
				return;
		}
		neighbors.push_back(neighbor);
	}
}
