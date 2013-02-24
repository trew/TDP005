/*
 * Grid.cpp
 *
 *  Created on: 22 feb 2013
 *      Author: Cosmic
 */

#include "Grid.h"
#include "Dijkstra.h"
#include "Define.h"

Grid::Grid() :
		start_tile(NULL), portal_tile(NULL), width(0), height(0) {
	pathfinding = new Dijkstra(this);
}

Grid::~Grid() {
	delete pathfinding;
	for (std::map<GridPosition, Tile*>::iterator it = tile_map.begin(); it != tile_map.end(); it++) {
		delete (*it).second;
	}
}

void connect(Tile* first, Tile* second) {
	/**
	 * Helper function to connect two neighbors
	 */
	if (first != NULL && second != NULL) {
		first->add_neighbor(second);
		second->add_neighbor(first);
	}
}

void Grid::connect_neighbors() {
	/**
	 * First it connects all inner tiles, then it connects all
	 */
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			Tile* north = tile_map[GridPosition(row - 1, col)];
			Tile* south = tile_map[GridPosition(row + 1, col)];
			Tile* west = tile_map[GridPosition(row, col - 1)];
			Tile* east = tile_map[GridPosition(row, col + 1)];
			Tile* me = tile_map[GridPosition(row, col)];
			connect(me, north);
			connect(me, south);
			connect(me, west);
			connect(me, east);
		}
	}
}

void Grid::create_grid(int _width, int _height) {
	width = _width;
	height = _height;

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			Tile* tile = new Tile(row, col);
			tile_map[GridPosition(row, col)] = tile;
			nodes.push_back(tile);
		}
	}
	connect_neighbors();
}

void Grid::add_tile(GridPosition position) {
	/**
	 * Adds a tile to the grid. Do not forget to add neighbors to this tile!
	 */
	if (tile_map.find(position) == tile_map.end()) {
		Tile* tile = new Tile(position.first, position.second);
		tile_map[position] = tile;
	}
}

Tile* Grid::get_tile(GridPosition position) {
	return tile_map[position];
}

Tile* Grid::get_tile(int row, int col) {
	return get_tile(GridPosition(row, col));
}

Tile* Grid::get_tile_from_mouse(int mouse_x, int mouse_y) {
	// first, snap to the top left corner of the tile, then divide
	// by TILESIZE to get the row/col version
	int x = (mouse_x - (mouse_x % TILESIZE));
	int y = (mouse_y - (mouse_y % TILESIZE));
	return get_tile(GridPosition(y / TILESIZE, x / TILESIZE));
}

bool Grid::is_placeable_tile(Tile* tile) {
	/**
	 * This function does not tell the whole truth whether a tile is placeable or not!
	 * The pathfinding algorithm must run in order to really ensure that a tower is
	 * allowed to be placed on this tile. This function only tells if there is already
	 * a tower on the tile or if the tile is the start/end tiles of the map.
	 */
	return tile->get_tower() == NULL && tile != get_start_tile()
			&& tile != get_portal_tile();
}

int Grid::get_tile_x_position(Tile* tile) {
	return tile->get_position().second * TILESIZE;
}

int Grid::get_tile_y_position(Tile* tile) {
	return tile->get_position().first * TILESIZE;
}

Tower* Grid::get_tower(GridPosition position) {
	return tile_map[position]->get_tower();
}

void Grid::set_tower(GridPosition position, Tower* tower) {
	tile_map[position]->set_tower(tower);
}

Path* Grid::get_path(Tile* start, Tile* dest) {
	return (Path*)(pathfinding->get_path(start, dest));
}

Tile* Grid::get_start_tile() {
	return start_tile;
}

Tile* Grid::get_portal_tile() {
	return portal_tile;
}

void Grid::set_start_tile(Tile* tile) {
	start_tile = tile;
}

void Grid::set_portal_tile(Tile* tile) {
	portal_tile = tile;
}

void Grid::clear_paths() {
	pathfinding->clear_saved_paths();
}

void Grid::reset() {
	for (std::map<GridPosition, Tile*>::iterator it = tile_map.begin(); it != tile_map.end(); it++) {
		if ((*it).second != NULL)
			(*it).second->set_tower(NULL);
	}
}

// private
NodeVector Grid::get_nodes() {
	return nodes;
}

