/*
 * Node.cpp
 *
 *  Created on: 3 dec 2010
 *      Author: samuel
 */

#include "Node.h"

const int int_max = 1000000;

Node::Node(int new_ID):
		parent(0), ID(new_ID),cost_from_start(int_max), allowed(true)
{
	///Constructor
}

Node::~Node()
{
	///Destructor
}

void Node::add_edge(Edge* new_edge)
{
	///Adds new edge to list of the current nodes edges.
	my_edges.push_back(new_edge);
}

void Node::set_allowed(bool new_val) {
	///Set node to be or not to be allowed
	allowed = new_val;
}

bool Node::is_allowed() {
	///Is node allowed to walk on?
	return allowed;
}

Node* Node::get_parent() {
	///Returns parent of node. Only returns good result if the Dijkstra algorithm has been run.
	return parent;
}

void Node::set_parent(Node* parent_in) {
	///Set new parent
	parent = parent_in;
}

int Node::get_ID() {
	///Returns current ID
	return ID;
}
int Node::get_cost_from_start() {
	///Returns cost from start
	return cost_from_start;
}


void Node::set_cost_from_start(int new_cost)
{
	///Set new cost from start
	cost_from_start = new_cost;
}


