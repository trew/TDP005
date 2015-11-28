/*
 * Tile.cpp
 *
 *  Created on: 22 feb 2013
 *      Author: Cosmic
 */

#include <Pathfinding/Tile.h>
#include <Entity/Tower/BaseTower.h>
#include <Core/Define.h>

Tile::Tile(int row, int col): position(GridPosition(row, col)), tower(NULL) {
}

Tile::~Tile() {
}

void Tile::set_tower(BaseTower* _tower) {
	if (tower != NULL && _tower == NULL) {
		// notify tower about us removing him from the tile
		tower->set_tile(NULL);
	}
	set_allowed(_tower == NULL);
	tower = _tower;
}

BaseTower* const Tile::get_tower() const {
	return tower;
}

const GridPosition& Tile::get_position() const {
	return position;
}

const float Tile::get_center_x() const {
	return position.second * TILESIZE + TILESIZE / 2.0f;
}

const float Tile::get_center_y() const {
	return position.first * TILESIZE + TILESIZE / 2.0f;
}

const int Tile::get_x_pixel_pos() const {
	return position.second * TILESIZE;
}

const int Tile::get_y_pixel_pos() const {
	return position.first * TILESIZE;
}
