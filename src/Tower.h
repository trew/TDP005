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

/**
 * \brief The Tower class
 *
 * This class handles the different kinds of towers in the game.
 */
class Tower: public Sprite {
public:
	Tower(int new_type, Tile* tile);			///<Constructor
	virtual ~Tower();							///<Destructor

public: // Selectors
	int get_sell_value() { return sell_value; } ///<Returns sell-value
	float get_range(); 							///<Returns range
	int get_level(); 							///<Returns level
	int get_damage(); 							///<Returns damage
	float get_boostmod(); 						///<Returns boostmod
	int get_cost_buy(); 						///<Returns cost (when buying)
	int get_cost_upgrade(); 					///<Returns upgrade-cost
	std::string conv_float_to_string(float i); 		///<Converts float to string
	std::string get_type_str(); 				///<Returns type as string
	std::string get_range_str();				///<Returns range as string
	std::string get_level_str();				///<Returns level as string
	std::string get_damage_str();				///<Returns damage as string
	std::string get_cost_buy_str();				///<Returns cost (when buying) as string
	std::string get_cost_upgrade_str();			///<Returns upgrade-cost as string
	double get_angle();							///<Returns current angle
	Sprite* get_current_target();				///<Returns current target

	void add_to_sell_value(int cost) { sell_value += cost; }///<Adds value to sell-value

public:
	void display_range(SDL_Surface* dest_surf, float x_pos, float y_pos); ///<Displays range as filled circle
	void draw(SDL_Surface* dest_surf); 				///<Draws the tower onto the destination surface
	void draw(SDL_Surface* dest_surf, int x, int y);///<Draws the tower onto the destination surface at (X,Y)

	void update_angle_to_target();					///<Updates the angle to current target
	void format_angle(double &angle);				///<Formats the angle to 0-360-format.
	void rotate(double value);						///<Rotates the tower

	bool target_in_sight();							///<Checks if tower is aiming at target
	bool target_in_range(Sprite *s);				///<Checks if target is in range
	void update_aim();								///<Rotates the cannon so that it aims more towards the current target.
	void find_new_target(EnemyList &enemy_list);	///<Finds new target for tower if possible
	void reload();									///<Handles reloadtimer
	bool is_loaded();								///<Checks if tower is loaded
	void shoot(ProjectileList& list);			///<Fires an projectile //TODO: projectile_list
	void apply_boost(float mod); ///<Applies boost to the towers stats

	bool has_this_target(Sprite* target);			///<Checks if the target is the towers current target
	void null_current_target();						///<Sets current target to NULL

	void update_boost(TowerList &tower_list);		///<Updates the towers boost-percentage and boosts the tower with apply_boost.
	void update(EnemyList &enemy_list);			///<Update tower state
	void shoot_if_possible(ProjectileList &projectile_list); ///<Shoots at the enemy if conditions are right.

	void set_selected();							///<Sets tower as selected
	void clear_selected();							///<Deselects tower if it is selected
	Sprite_List get_infosprites();					///<Returns sprites containing text-information
	void update_info_sprites();						///<Updates the text-information stored in the sprites

	bool upgrade(int new_type);						///<Upgrades the tower to the new type and changes its stats thereafter.

	Tile* get_tile();
	void set_tile(Tile*);

private:
	void init_info_sprites();						///<Initializes the information-sprites

private: // Properties
	float 	range;				// Tower Range
	float 	base_range;
	float 	rotation_speed;	    // Cannon Rotationspeed
	int 	spread;				// Angle-deviation-acceptance for when to fire at enemy
	float	reloading_time;		// The tower's rate of fire
	int 	damage;				// Attackdamage
	int 	base_damage;
	float	projectile_speed;	// Speed of the projectile being shot
	int 	level;				// Current level
	int		cost_buy;			// Cost to build
	int 	cost_upgrade;		// Cost to upgrade
	float 	boostmod;			// Boost modifier for boost towers
	int 	sell_value;			// Value when selling
	Tile*	tile;

private: // Specific variables for calculations etc.
	SDL_Surface* 	base_surf;
	SDL_Surface* 	cannon_surf;
	Enemy*			current_target;
	double			current_angle;
	double			target_angle;
	double			rotation_modifier;
	int				reload_counter;
	int				smoothing;
	float			boost_modifier;
};

#endif /* TOWER_H_ */
