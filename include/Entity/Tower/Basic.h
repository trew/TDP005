/*
 * Basic.h
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#ifndef BASIC_H_
#define BASIC_H_

#include <Entity/Tower/Tower.h>

namespace towers {

class Basic: public towers::Tower {
public:
	Basic();
	virtual ~Basic();

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
#endif /* BASIC_H_ */
