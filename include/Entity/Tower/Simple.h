/*
 * Simple.h
 *
 *  Created on: 25 feb 2013
 *      Author: Cosmic
 */

#ifndef SIMPLE_H_
#define SIMPLE_H_

#include <Entity/Tower/Tower.h>
#include <string>

namespace towers {

class Simple: public towers::Tower {
public:
	Simple(SDL_Renderer*);
	virtual ~Simple();

	TowerType get_type();
	std::string get_type_str();
	bool upgrade(TowerType type); 	   ///< type is the towertype to be upgraded to

	Projectile* spawn_projectile(Game* g, float x, float y, float angle);
	void update_informationtext();
};

} /* namespace towers */
#endif /* SIMPLE_H_ */
