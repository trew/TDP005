/*
 * Dijkstra.cpp
 *
 *  Created on: 30 nov 2010
 *      Author: samuel
 */

#include "Dijkstra.h"
#include <iostream>

const int max_cost = 99999;

Dijkstra::Dijkstra(Grid* grid) :
		grid(grid) {
}

Dijkstra::~Dijkstra() {
}

NodePath* Dijkstra::calculate_path(Node* start, Node* dest) {
	/**
	 * This is the dijkstra algorithm that searches through the node graph for
	 * the most efficiant way from start node to destination node.
	 */

	//Initialize; set cost to startnode to zero and rest to inf.
	Node* current_node = start;
	NodeVector graph = grid->get_nodes();

	for (NodeVector::iterator it = graph.begin(); it != graph.end(); it++) {
		(*it)->set_cost_from_start(max_cost);
	}
	start->set_cost_from_start(0);
	prio_queue.push(current_node);

	//Core loop
	while (!prio_queue.empty()) {
		current_node = prio_queue.top();
		prio_queue.pop();
		if (current_node == dest) {
			save_path(start, dest);
			return create_path_copy(saved_paths[NodePair(start, dest)]);
		}

		NodeVector neighbors = current_node->get_neighbors();
		for (NodeVector::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++) {
			if (*neighbor == current_node)
				continue;
			if (!(*neighbor)->is_allowed())
				continue;
			int new_cost = current_node->get_cost_from_start();
			new_cost++;
			int neighbor_cost = (*neighbor)->get_cost_from_start();

			if (new_cost < neighbor_cost) {
				(*neighbor)->set_cost_from_start(new_cost);
				(*neighbor)->set_parent(current_node);
				prio_queue.push((*neighbor));
			}
		}
	}
	save_path(start, dest);
	return create_path_copy(saved_paths[NodePair(start, dest)]);
}

void Dijkstra::clear_saved_paths() {
	for (std::map<NodePair, NodePath*>::iterator it = saved_paths.begin();
			it != saved_paths.end(); it++) {
		delete (*it).second;
	}
	saved_paths.clear();
	NodeVector nodes = grid->get_nodes();
	for (NodeVector::iterator node = nodes.begin(); node != nodes.end(); node++) {
		(*node)->set_cost_from_start(max_cost);
	}
}

NodePath* Dijkstra::create_path_copy(NodePath* path) {
	/**
	 * Creates a copy of the path with pointers to the nodes
	 */
	NodePath* new_path = new NodePath;
	for (NodePath::iterator it = path->begin(); it != path->end(); it++) {
		new_path->push_back(*it);
	}
	return new_path;
}

void Dijkstra::save_path(Node* start, Node* dest) {
	NodePath* path = new NodePath;
	Node* current_node = dest;
	if (current_node->get_cost_from_start() == max_cost) {
		saved_paths[NodePair(start, dest)] = path;
		return;
	}
	while (start != current_node) {
		path->push_back(current_node);
		current_node = current_node->get_parent();
	}
	saved_paths[NodePair(start, dest)] = path;
}

NodePath* Dijkstra::get_path(Node* start, Node* dest) {
	/**
	 * After running the find_paths function, you can here extract the exact way from start to destination.
	 */

	if (saved_paths.find(NodePair(start, dest)) != saved_paths.end()) {
		return create_path_copy(saved_paths[NodePair(start, dest)]);
	}

	// otherwise calculate a new path
	return calculate_path(start, dest);
}
