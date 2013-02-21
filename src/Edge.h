/*
 * Edge.h
 *
 *  Created on: 3 dec 2010
 *      Author: samuel
 */

#ifndef EDGE_H_
#define EDGE_H_

#include <vector>
class Node;
typedef std::vector<Node*> node_graph;

/** \brief Edges connects nodes with each other.
 *
 * Edges connects nodes with each other. They are the pieces in the Dijkstra algorithm that contains the way cost.
 * In our game the cost is always 1, but it could easily be expanded.
 */

class Edge {
public:
	Edge();
	virtual ~Edge();

	int get_cost();	///Get edge cost

	void add_node(Node*);	///Add connected node

	node_graph my_nodes;	///List of connected nodes
private:
	int cost;
};
#endif /* EDGE_H_ */
