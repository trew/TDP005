/*
 * Speed.h
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#ifndef SPEED_H_
#define SPEED_H_

#include <Entity/Tower/Tower.h>

namespace towers {

class Speed: public towers::Tower {
public:
	Speed();
	virtual ~Speed();

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
#endif /* SPEED_H_ */
