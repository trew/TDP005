/*
 * Enemy.h
 *
 *  Created on: 18 nov 2010
 *      Author: samuel
 */

#ifndef ENEMY_H_
#define ENEMY_H_

#include "Sprite.h"
#include "Dijkstra.h"
#include "Game.h"
#include <vector>

typedef std::list<std::pair<int, int> > path_list;

/** \brief Enemy class
 *
 * Enemies are units that walk from the left part of the map to the right.
 * They have hitpoints and movement speed to determine their difficulty.
 * The give reward in form of money and score points.
 */

class Enemy: public Sprite {
public:

	Enemy(int new_type, int x_pos_in, int y_pos_in, int width_in, int height_in, int new_level);	///< Constructor
	virtual ~Enemy();			///< Destructor

	void draw(SDL_Surface* dest_surf);					///< Draw image

	void update(Sprite_List &enemy_list);				///< Update state of enemy
	void take_damage(int dmg);							///< Recieve damage
	bool is_killed() { return killed; }					///< Is enemy killed by player?
	bool has_reached_goal() { return reached_goal; }	///< Has enemy reached its goal?

	bool can_update_path();								///< Is there a way to reach goal?
	void update_path();									///< Set new path
	void new_path(std::list<int> int_path);				///< Convert int path to pairs

	int get_cost_buy() { return cost; }					///< Cost of enemy. Used in Level
	int get_reward_money() { return reward_money; }		///< Recieve money from enemy
	int get_reward_score() { return reward_score; }		///< Recieve score from enemy

private:
	path_list my_path;				///< List if current path
	std::list<int> new_path_int;	///< New path returned from dijkstra

	std::pair<int, int> conv_int_to_XY(int in);	///< Convert an int to appropriate <X,Y>-pair.
	int move_dir();		///< Move direction
	void move();		///< Execute movement

	int x_vel, y_vel;
	int move_speed;
	int health;
	int max_health;
	bool reached_goal;
	bool killed;
	int level;
	int cost;

	int reward_money;
	int reward_score;

};

#endif /* ENEMY_H_ */
