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
#include <Pathfinding/Grid.h>
#include <Pathfinding/Node.h>

typedef std::list<Node*> NodePath;

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
public:
	Dijkstra(Grid*);		///Constructor
	virtual ~Dijkstra();	///Destructor

	NodePath* get_path(Node* start, Node* dest);	///Get path from start node to destination node.

	void clear_saved_paths();
private:
	NodePath* calculate_path(Node* start, Node* dest);
	void save_path(Node* start, Node* dest);

	NodePath* create_path_copy(NodePath* path);

	std::map<NodePair, NodePath*> saved_paths;

	std::priority_queue<Node*, NodeVector, CompareCost> prio_queue;

	Grid* grid;
};


#endif /* DIJKSTRA_H_ */
