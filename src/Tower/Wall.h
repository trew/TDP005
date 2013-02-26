/*
 * Wall.h
 *
 *  Created on: 25 feb 2013
 *      Author: Cosmic
 */

#ifndef WALL_H_
#define WALL_H_

#include "Tower.h"

namespace towers {

class Wall: public towers::Tower {
public:
	Wall();
	virtual ~Wall();

	void update_informationtext();

	TowerType get_type();
	std::string get_type_str();
	bool upgrade(TowerType type); 	   ///< type is the towertype to be upgraded to
};

} /* namespace towers */
#endif /* WALL_H_ */
