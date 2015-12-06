/*
 * Tower.h
 *
 *  Created on: 17 nov 2010
 *      Author: samuel
 */

#ifndef BASE_TOWER_H_
#define BASE_TOWER_H_

#include <Entity/Sprite.h>
#include <UI/Sound.h>
#include <Pathfinding/Tile.h>
#include <UI/Text.h>
#include <map>
#include <list>
#include <vector>
#include <Entity/Tower/Tower.h>

/**
 * \brief The Tower class
 *
 * This class handles the different kinds of towers in the game.
 */
class BaseTower: public Sprite {
public:
	BaseTower(Game* game, towers::TowerType, Tile* tile);			///<Constructor
	virtual ~BaseTower();							///<Destructor

public: // Selectors
	double get_angle();							///<Returns current angle

public:
	void drawRange(SDL_Renderer* renderer); ///<Displays range as filled circle
	void drawRange(SDL_Renderer* renderer, float xPos, float yPos); ///<Displays range as filled circle
	void draw(SDL_Renderer* renderer); 				///<Draws the tower onto the destination surface
	void draw(SDL_Renderer* renderer, int x, int y);///<Draws the tower onto the destination surface at (X,Y)

	void update_angle_to_target();					///<Updates the angle to current target
	void format_angle(double &angle);				///<Formats the angle to 0-360-format.
	void rotate(double value);						///<Rotates the tower

	bool target_in_sight();							///<Checks if tower is aiming at target
	bool target_in_range(Enemy*);				///<Checks if target is in range
	void update_aim(const float& timeStep);								///<Rotates the cannon so that it aims more towards the current target.
	void find_new_target();							///<Finds new target for tower if possible
	void reload(const float& timeStep);									///<Handles reloadtimer
	bool is_loaded();								///<Checks if tower is loaded
	void try_shoot();									///<Fires an projectile
	void apply_boost(float mod); ///<Applies boost to the towers stats

	Sprite* get_target();				///<Returns current target
	void set_target(Enemy* target);

	void update_boost();		///<Updates the towers boost-percentage and boosts the tower with apply_boost.
	void update(const float& timeStep) override;			///<Update tower state

	void set_selected(bool selected);				///<Sets tower as selected
	Sprite_List* get_infosprites();					///<Returns sprites containing text-information

	bool upgrade(towers::TowerType type);						///<Upgrades the tower to the new type and changes its stats thereafter.

	Tile* get_tile();
	void set_tile(Tile*);

	/* Implementation details */
	int get_base_damage();
	float get_projectile_speed();
	const float get_reloading_time();
	int get_spread();
	float get_rotation_speed();
	float get_range_in_pixels();
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
	SDL_Renderer* renderer;

private: // Specific variables for calculations etc.
	Enemy*			current_target;
	double			current_angle;
	double			old_angle; //the angle held last frame, determines whether to remake the surface using rotozoom
	double			target_angle;
	double			rotation_modifier;
	float				reload_timer;
	bool 			loaded;
	int				smoothing;
};

#endif /* BASE_TOWER_H_ */
