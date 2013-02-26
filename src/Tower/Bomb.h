/*
 * Bomb.h
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#ifndef BOMB_H_
#define BOMB_H_

#include "Tower.h"

namespace towers {

class Bomb: public towers::Tower {
public:
	Bomb();
	virtual ~Bomb();

	void update_informationtext();
	Projectile* spawn_projectile(Game* g, float x, float y, float angle);
	std::string get_type_str();
	TowerType get_type();
	bool upgrade(TowerType);
private:
	TowerData level_1;
	TowerData level_2;
	TowerData level_3;
};


} /* namespace towers */
#endif /* BOMB_H_ */
