/*
 * Dijkstra.cpp
 *
 *  Created on: 30 nov 2010
 *      Author: samuel
 */

#include "Dijkstra.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

const int int_max = 10000;

Dijkstra::Dijkstra() {
}

Dijkstra::~Dijkstra()
{
	for (node_graph::iterator it = graph.begin(); it != graph.end(); it++)
	{
		delete (*it);
	}
}

Node* Dijkstra::get_node(int node_ID) {
	/**
	 * Returns the node of a specific position.
	 */
	for(node_graph::iterator it = graph.begin(); it != graph.end(); it++) {
		if((*it)->get_ID() == node_ID) return (*it);
	}
	return NULL;
}

void Dijkstra::update_graph(std::map<int, Sprite*> grid){
	/**
	 * If a tower is blocking the way, you need this function to disable that node.
	 *
	 */
	for(std::map<int, Sprite*>::iterator it = grid.begin(); it != grid.end(); it++) {
		if( ((*it).second != NULL) ) {
			if ( get_node((*it).first / TILESIZE) != NULL )
				get_node( ((*it).first / TILESIZE) )->set_allowed(false);
		} else {
			if ( get_node((*it).first / TILESIZE) != NULL )
				get_node( ((*it).first / TILESIZE) )->set_allowed(true);
		}
	}
}


void Dijkstra::create_graph(std::map<int, Sprite*> grid, int height, int width)
{
	/**
	 * Creates a complete graph of a rectangular grid system containing nodes and edges.
	 * The nodes are connected in four directions: up, down, left and right.
	 * Nodes on the top are also not connected to nodes on the bottom.
	 *
	 */
	graph.clear();

	/* Start creating nodes */
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			graph.push_back(new Node( (x)+((y)*height) ) );
		}
	}
	/* End creating nodes */

	/* Start creating edges */
	for(int y = 0; y < height-1; y++) {
		for(int x = 0; x < width-1; x++) {
			Edge* temp_edge_hor = new Edge;
			Edge* temp_edge_ver = new Edge;

			temp_edge_hor->add_node(graph[ (x) + ((y*height))     ]);
								 graph[ (x) + ((y*height))     ]->add_edge(temp_edge_hor);
			temp_edge_hor->add_node(graph[ ((x+1)) + ((y*height)) ]);
								 graph[ ((x+1)) + ((y*height)) ]->add_edge(temp_edge_hor);

			temp_edge_ver->add_node(graph[(x) + ((y*height))]);
								 graph[(x) + ((y*height))]->add_edge(temp_edge_ver);
			temp_edge_ver->add_node(graph[(x) + ( ((y+1)*height))  ]);
								 graph[(x) + ( ((y+1)*height))  ]->add_edge(temp_edge_ver);
		}
	}

	for(int x = 0; x < width -1; x++) {
		Edge* temp_edge_hor = new Edge;
		temp_edge_hor->add_node(graph[(x) + (((height-1)) *height) ]);
							 graph[(x) + (((height-1)) *height) ]->add_edge(temp_edge_hor);
		temp_edge_hor->add_node(graph[((x+1)) + (((height-1)) *height)]);
							 graph[((x+1)) + (((height-1)) *height)]->add_edge(temp_edge_hor);
	}
	for(int y = 0; y < height -1; y++) {
		Edge* temp_edge_ver = new Edge;
		temp_edge_ver->add_node(graph[((width-1)) + ((y*height))]);
							 graph[((width-1)) + ((y*height))]->add_edge(temp_edge_ver);
		temp_edge_ver->add_node(graph[((width-1)) + (((y+1)*height))]);
						     graph[((width-1)) + (((y+1)*height))]->add_edge(temp_edge_ver);
	}

	update_graph(grid);
/* End creating edges */

}

std::list<int> Dijkstra::find_paths(std::map<int, Sprite*> grid, int start, int dest) {
	/// Calls the other find_paths function with converted parameters.
	return find_paths(grid, get_node(start), get_node(dest));
}

std::list<int> Dijkstra::find_paths(std::map<int, Sprite*> grid, Node* start, Node* dest)
{
	/**
	 * This is the dijkstra algorithm that searches through the node graph for
	 * the most efficiant way from start node to destination node.
	 */
	update_graph(grid);

	//Initialize; set cost to startnode to zero and rest to inf.
	Node* current_node = start;
	for (node_graph::iterator it = graph.begin(); it != graph.end(); it++)
	{
		(*it)->set_cost_from_start(int_max);
	}
	start->set_cost_from_start(0);
	prio_queue.push(current_node);
	//Core loop

	while (!prio_queue.empty())
	{
		current_node = prio_queue.top();
		prio_queue.pop();
		for (edge_graph::iterator iter_edge = current_node->my_edges.begin(); iter_edge != current_node->my_edges.end(); iter_edge++)
		{
			if(current_node == dest)
			{
				return get_path(start, dest);
			}
			for (node_graph::iterator iter_node = (*iter_edge)->my_nodes.begin(); iter_node != (*iter_edge)->my_nodes.end(); iter_node++)
			{
				if(!(*iter_node)->is_allowed())
					break;
				int new_cost = current_node->get_cost_from_start() + (*iter_edge)->get_cost();

				if ((new_cost < (*iter_node)->get_cost_from_start() ) && current_node->get_ID() != (*iter_node)->get_ID())
				{
					(*iter_node)->set_cost_from_start(new_cost);
					(*iter_node)->set_parent(current_node);
					prio_queue.push((*iter_node));
				}
			}
		}
	}

	return get_path(start, dest);
}

std::list<int> Dijkstra::get_path(Node* start, Node* dest)
{
	/**
	 * After running the find_paths function, you can here extract the exact way from start to destination.
	 */
	std::list<int> return_list;
	Node* current_node = dest;
	if (current_node->get_cost_from_start() == int_max) {
		return return_list;
	}
	while (start != current_node)
	{
		return_list.push_back(current_node->get_ID() * TILESIZE);
		current_node = current_node->get_parent();
	}
	return return_list;
}
