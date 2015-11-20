/*
 * Tile.h
 *
 *  Created on: 22 feb 2013
 *      Author: Cosmic
 */

#ifndef TILE_H_
#define TILE_H_

#include <Pathfinding/Node.h>

class BaseTower;

class Tile : public Node {
	friend class Grid;
public:
	virtual ~Tile();

	BaseTower* get_tower();
	void set_tower(BaseTower*);

	GridPosition get_position();
	float get_center_x();
	float get_center_y();
	int get_x_pixel_pos();
	int get_y_pixel_pos();
private:
	Tile(int row, int col);
	GridPosition position;

	BaseTower* tower;
};

#endif /* TILE_H_ */
