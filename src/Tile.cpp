/*
 * Tile.cpp
 *
 *  Created on: 22 feb 2013
 *      Author: Cosmic
 */

#include "Tile.h"
#include "Tower.h"
#include "Define.h"

Tile::Tile(int row, int col): position(GridPosition(row, col)), tower(NULL) {
}

Tile::~Tile() {
}

void Tile::set_tower(Tower* _tower) {
	if (tower != NULL && _tower == NULL) {
		// notify tower about us removing him from the tile
		tower->set_tile(NULL);
	}
	set_allowed(_tower == NULL);
	tower = _tower;
}

Tower* Tile::get_tower() {
	return tower;
}

GridPosition Tile::get_position() {
	return position;
}

float Tile::get_center_x() {
	return position.second * TILESIZE + TILESIZE / 2.0f;
}

float Tile::get_center_y() {
	return position.first * TILESIZE + TILESIZE / 2.0f;
}

int Tile::get_x_pixel_pos() {
	return position.second * TILESIZE;
}

int Tile::get_y_pixel_pos() {
	return position.first * TILESIZE;
}
