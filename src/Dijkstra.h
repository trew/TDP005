/*
 * Dijkstra.h
 *
 *  Created on: 30 nov 2010
 *      Author: samuel
 */

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <vector>
#include <queue>
#include <list>
#include <map>
#include "Sprite.h"
#include "Node.h"
#include "Edge.h"

/** \brief Used by dijkstra
 *
 * Used by dijkstra class to compare costs.
 */
class CompareCost {
public:
	///Operator
	bool operator() (Node* node1, Node* node2) {
		return (node1->get_cost_from_start() > node2->get_cost_from_start());
	}
};

/** \brief Dijkstra algorithm
 *
 * The Dijkstra algorithm helps enemies find the most efficiant way to their destination.
 */

class Dijkstra {
	friend class Edge;
public:
	Dijkstra();		///Constructor
	virtual ~Dijkstra();	///Destructor
	Node* get_node(int node_ID);	///Returns a node

	void create_graph(std::map<int, Sprite*> grid, int height, int width);	/// Creates a graph system
	void update_graph(std::map<int, Sprite*> grid);							/// Disables nodes depending on grid

	std::list<int> find_paths(std::map<int, Sprite*> grid, Node* start, Node* dest);
	std::list<int> find_paths(std::map<int, Sprite*> grid, int start, int dest);	///
	std::list<int> get_path(Node* start, Node* dest);						///Get path from start node to destination node.

private:
	std::priority_queue<Node*, node_graph, CompareCost> prio_queue;

	node_graph graph;
	static edge_graph edges;
};


#endif /* DIJKSTRA_H_ */
