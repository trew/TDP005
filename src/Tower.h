/*
 * Tower.h
 *
 *  Created on: 17 nov 2010
 *      Author: samuel
 */

#ifndef TOWER_H_
#define TOWER_H_

#include "Sprite.h"
#include "Sound.h"
#include "Tile.h"
#include "Text.h"
#include <map>
#include <list>
#include <vector>
#include "Tower/Tower.h"

/**
 * \brief The Tower class
 *
 * This class handles the different kinds of towers in the game.
 */
class Tower: public Sprite {
public:
	Tower(towers::TowerType, Tile* tile);			///<Constructor
	virtual ~Tower();							///<Destructor

public: // Selectors
	double get_angle();							///<Returns current angle

public:
	void draw_range(SDL_Surface* dest_surf); ///<Displays range as filled circle
	void draw_range(SDL_Surface* dest_surf, float x_pos, float y_pos); ///<Displays range as filled circle
	void draw(SDL_Surface* dest_surf); 				///<Draws the tower onto the destination surface
	void draw(SDL_Surface* dest_surf, int x, int y);///<Draws the tower onto the destination surface at (X,Y)

	void update_angle_to_target();					///<Updates the angle to current target
	void format_angle(double &angle);				///<Formats the angle to 0-360-format.
	void rotate(double value);						///<Rotates the tower

	bool target_in_sight();							///<Checks if tower is aiming at target
	bool target_in_range(Enemy*);				///<Checks if target is in range
	void update_aim();								///<Rotates the cannon so that it aims more towards the current target.
	void find_new_target(EnemyList &enemy_list);	///<Finds new target for tower if possible
	void reload();									///<Handles reloadtimer
	bool is_loaded();								///<Checks if tower is loaded
	void shoot(ProjectileList& list);			///<Fires an projectile //TODO: projectile_list
	void apply_boost(float mod); ///<Applies boost to the towers stats

	Sprite* get_target();				///<Returns current target
	void set_target(Enemy* target);

	void update_boost(TowerList &tower_list);		///<Updates the towers boost-percentage and boosts the tower with apply_boost.
	void update(EnemyList &enemy_list);			///<Update tower state
	void shoot_if_possible(ProjectileList &projectile_list); ///<Shoots at the enemy if conditions are right.

	void set_selected(bool selected);				///<Sets tower as selected
	Sprite_List* get_infosprites();					///<Returns sprites containing text-information

	bool upgrade(towers::TowerType type);						///<Upgrades the tower to the new type and changes its stats thereafter.

	Tile* get_tile();
	void set_tile(Tile*);

	/* Implementation details */
	uint get_base_damage();
	float get_projectile_speed();
	float get_reloading_time();
	int get_spread();
	float get_rotation_speed();
	float get_range();
	float get_base_range();
	float get_boostmod();
	towers::TowerType get_type();

	int get_cost_buy();
	int get_cost_upgrade();
	void set_sell_value(int value);
	int get_sell_value();
	int get_level();
	int get_max_level();

	void set_range(float range);
	void set_damage(int damage);
private: // Properties
	Tile*	tile;
	towers::Tower* twr_impl;

private: // Specific variables for calculations etc.
	Enemy*			current_target;
	double			current_angle;
	double			target_angle;
	double			rotation_modifier;
	int				reload_counter;
	int				smoothing;
	float			boost_modifier;
};

#endif /* TOWER_H_ */
