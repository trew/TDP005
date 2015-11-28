/*
 * Grid.h
 *
 *  Created on: 22 feb 2013
 *      Author: Cosmic
 */

#ifndef GRID_H_
#define GRID_H_

#include <Pathfinding/Tile.h>
#include <list>
#include <map>

class BaseTower;
class Tile;
class Dijkstra;
typedef std::pair<int, int> GridPosition; ///< Position on the grid on Row/Col format
typedef std::list<Tile*> Path;

class Grid {
public:
	Grid();
	virtual ~Grid();

	void create_grid(int width, int height);
	void add_tile(const GridPosition &pos);

	Tile* get_tile(const GridPosition &);
	Tile* get_tile(int row, int col);
	Tile* get_tile_from_mouse(int mouse_x, int mouse_y);
	bool is_placeable_tile(Tile*) const;

	int get_tile_x_position(Tile*) const; ///< Returns the X position in pixels
	int get_tile_y_position(Tile*) const; ///< Returns the Y position in pixels


	BaseTower* get_tower(const GridPosition &);
	void set_tower(const GridPosition &, BaseTower*);

	Path* get_path(Tile* start, Tile* dest);

	Tile* get_start_tile() const;
	Tile* get_portal_tile() const;
	void set_start_tile(Tile*);
	void set_portal_tile(Tile*);

	void clear_paths();
	void reset();

	NodeVector get_nodes();
private:
	void connect_neighbors();

	Tile* start_tile;
	Tile* portal_tile;
	Dijkstra* pathfinding;
	std::map<GridPosition, Tile*> tile_map;

	int width;
	int height;

	NodeVector nodes;

};

#endif /* GRID_H_ */
