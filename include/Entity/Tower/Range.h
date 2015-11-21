/*
 * Range.h
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#ifndef RANGE_H_
#define RANGE_H_

#include <Entity/Tower/Tower.h>

namespace towers {

class Range: public towers::Tower {
public:
	Range(SDL_Renderer*);
	virtual ~Range();

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
#endif /* RANGE_H_ */
