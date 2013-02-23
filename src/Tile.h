/*
 * Tile.h
 *
 *  Created on: 22 feb 2013
 *      Author: Cosmic
 */

#ifndef TILE_H_
#define TILE_H_

#include "Node.h"

class Tower;

class Tile : public Node {
	friend class Grid;
public:
	virtual ~Tile();

	Tower* get_tower();
	void set_tower(Tower*);

	GridPosition get_position();
	int get_x_pixel_pos();
	int get_y_pixel_pos();
private:
	Tile(int row, int col);
	GridPosition position;

	Tower* tower;
};

#endif /* TILE_H_ */
