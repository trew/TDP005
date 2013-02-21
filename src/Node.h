/*
 * Node.h
 *
 *  Created on: 3 dec 2010
 *      Author: samuel
 */

#ifndef NODE_H_
#define NODE_H_

#include <vector>

class Edge;
typedef std::vector<Edge*> edge_graph;


/** \brief Node class
 *
 * Nodes in the grid system contains info about cost from start and it also holds its edges.
 */

class Node {
public:
	Node(int);	///Constructor
	virtual ~Node();	///Destructor

	void add_edge(Edge*);	///Add new edge

	int get_ID();			///Returns ID of node

	void set_allowed(bool new_val);		///Set node allowed
	bool is_allowed();					///Is node allowed to walk on?

	Node* get_parent();					///Return parent node
	void set_parent(Node* parent_in);	///Set new parent

	int get_cost_from_start();			///Returns cost from start
	void set_cost_from_start(int new_cost);	///Set new cost from start

    edge_graph my_edges;				///List of connecting edges
private:
	Node* parent;
	int ID;
	int cost_from_start;

    bool allowed;
};
#endif /* NODE_H_ */
