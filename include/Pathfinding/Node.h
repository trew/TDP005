/*
 * Node.h
 *
 *  Created on: 3 dec 2010
 *      Author: samuel
 */

#ifndef NODE_H_
#define NODE_H_

#include <vector>

class Node;
typedef std::vector<Node*> NodeVector;
typedef std::pair<Node*, Node*> NodePair;
typedef std::pair<int, int> GridPosition;

/** \brief Node class
 *
 * Nodes in the grid system contains info about cost from start and it also holds its edges.
 */

class Node {
public:
	Node();
	virtual ~Node();	///Destructor

	void set_allowed(bool new_val);		///Set node allowed
	bool is_allowed();					///Is node allowed to walk on?

	Node* get_parent();					///Return parent node
	void set_parent(Node* parent_in);	///Set new parent

	int get_cost_from_start();			///Returns cost from start
	void set_cost_from_start(int new_cost);	///Set new cost from start

    NodeVector get_neighbors();
    void add_neighbor(Node*);
private:
	Node* parent; ///< Used for the Dijkstra algorithm
	int cost_from_start;
    bool allowed;

	NodeVector neighbors;
};
#endif /* NODE_H_ */
