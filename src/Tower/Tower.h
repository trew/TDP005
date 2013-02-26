/*
 * Tower.h
 *
 *  Created on: 25 feb 2013
 *      Author: Cosmic
 */

#ifndef TOWER_H
#define TOWER_H

#include <string>
#include <SDL/SDL.h>
#include "../Sprite.h"
#include "../Projectile.h"

namespace towers {

enum TowerType {
	WALL,
	SIMPLE,
	BASIC,
	SPEED,
	RANGE,
	BOMB,
	BOOST
};

typedef struct {
	float range;
	uint damage;
	uint max_level;
	float rotation_speed;
	float reloading_time;
	float projectile_speed;
	uint cost_buy;
	uint cost_upgrade;
	float boostmod;
	std::string base_surface_location;
	std::string cannon_surface_location;
} TowerData;


class Tower {
public:
	virtual ~Tower();

	/* Required Virtuals */
	virtual TowerType get_type()=0;
	virtual std::string get_type_str()=0;
	virtual bool upgrade(TowerType type)=0; 	   ///< type is the towertype to be upgraded to

	/* Optional Virtuals */
	virtual Projectile* spawn_projectile(float x, float y, float angle);
	virtual void update_informationtext(); ///< each sprite represents a line

	/* Common Public interface */
	void update_data(TowerData data);
	Sprite_List* get_informationtext();

	uint get_level();
	uint get_max_level();
	uint get_damage();
	void set_damage(uint damage);
	float get_range();
	void set_range(float range);
	uint get_base_damage();
	float get_base_range();
	float get_boostmod();
	float get_projectile_speed();
	float get_rotation_speed();
	int get_spread();
	float get_reloading_time();

	uint get_sell_value();
	void set_sell_value(uint value);

	uint get_cost_buy();
	uint get_cost_upgrade();

	std::string get_range_str();
	std::string get_level_str();
	std::string get_damage_str();
	std::string get_cost_buy_str();
	std::string get_cost_upgrade_str();
	std::string get_boostmod_str();

	SDL_Surface* get_base_surface();
	SDL_Surface* get_cannon_surface();

protected:
	Tower();

	SDL_Surface* 	base_surf;
	SDL_Surface* 	cannon_surf;

	float base_range;		// Tower Range
	uint base_damage;		// Attackdamage
	uint max_level;
	float rotation_speed;	// Cannon Rotationspeed
	float reloading_time;	// The tower's rate of fire
	float projectile_speed;	// Speed of the projectile being shot
	uint cost_buy;			// Cost to build
	int spread;				// Angle-deviation-acceptance for when to fire at enemy

	uint level;				// Current level
	float range; 			// Tower Range
	uint damage;				// Attackdamage
	uint cost_upgrade;		// Cost to upgrade
	float boostmod;			// Boost modifier for boost towers
	uint sell_value;		// Value when selling

	std::string conv_float_to_string(float f); 		///<Converts float to string
	std::string conv_int_to_string(int i); 			///<Converts int to string

	void set_base_surf(SDL_Surface*);
	void set_cannon_surf(SDL_Surface*);

	void clear_informationtext();
	void add_row_to_information_text(std::string); ///< Helper function

	static float format_range(float);
private:
	Sprite_List* infotext;
};

} /* namespace towers */
#endif /* TOWER_H_ */
