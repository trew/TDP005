/*
 * Tower.h
 *
 *  Created on: 25 feb 2013
 *      Author: Cosmic
 */

#ifndef TOWER_H
#define TOWER_H

#include <string>
#include <SDL.h>
#include <Entity/Sprite.h>
#include <Entity/Projectile.h>

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
	Uint32 damage;
	Uint32 max_level;
	float rotation_speed;
	float reloading_time;
	float projectile_speed;
	Uint32 cost_buy;
	Uint32 cost_upgrade;
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
	virtual Projectile* spawn_projectile(Game* g, float x, float y, float angle);
	virtual void update_informationtext(); ///< each sprite represents a line

	/* Common Public interface */
	void update_data(TowerData data);
	Sprite_List* get_informationtext();

	Uint32 get_level();
	Uint32 get_max_level();
	Uint32 get_damage();
	void set_damage(Uint32 damage);
	float get_range_in_pixels();
	float get_range();
	void set_range(float range);
	Uint32 get_base_damage();
	float get_base_range();
	float get_boostmod();
	float get_projectile_speed();
	float get_rotation_speed();
	int get_spread();
	const float get_reloading_time() const;
	const float get_base_reloading_time() const;
	void set_reloading_time(const float& t);

	void apply_boost(float mod);

	Uint32 get_sell_value();
	void set_sell_value(Uint32 value);

	Uint32 get_cost_buy();
	Uint32 get_cost_upgrade();

	SDL_Texture* getBaseTexture();
	SDL_Texture* getBaseCannonTexture();
	SDL_Texture* getCannonTexture();

	std::string ftos(float f); 		///<Converts float to string
	std::string itos(int i); 			///<Converts int to string

	void setCannonTexture(SDL_Texture*);
protected:
	Tower(SDL_Renderer*);

	SDL_Renderer* renderer; //reference to game renderer in order to generate texts
	SDL_Texture* 	baseTexture;
	SDL_Texture*	baseCannonTexture;
	SDL_Texture* 	cannonTexture;

	float base_range;		// Tower Range
	Uint32 base_damage;		// Attackdamage
	float reloading_time;	// The tower's rate of fire
	float base_reloading_time;
	Uint32 max_level;
	float rotation_speed;	// Cannon Rotationspeed
	float projectile_speed;	// Speed of the projectile being shot
	Uint32 cost_buy;			// Cost to build
	int spread;				// Angle-deviation-acceptance for when to fire at enemy

	Uint32 level;				// Current level
	float range; 			// Tower Range
	Uint32 damage;				// Attackdamage
	Uint32 cost_upgrade;		// Cost to upgrade
	float boostmod;			// Boost modifier for boost towers
	Uint32 sell_value;		// Value when selling

	void setBaseTexture(SDL_Texture*);
	void setBaseCannonTexture(SDL_Texture*);

	void clear_informationtext();
	void add_row_to_information_text(std::string); ///< Helper function

	static float format_range(float);
private:
	Sprite_List* infotext;
};

} /* namespace towers */
#endif /* TOWER_H_ */
