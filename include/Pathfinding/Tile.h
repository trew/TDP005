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

	BaseTower* const get_tower() const;
	void set_tower(BaseTower* const);

	const GridPosition& get_position() const;
	const float get_center_x() const;
	const float get_center_y() const;
	const int get_x_pixel_pos() const;
	const int get_y_pixel_pos() const;
private:
	Tile(int row, int col);
	GridPosition position;

	BaseTower* tower;
};

#endif /* TILE_H_ */
