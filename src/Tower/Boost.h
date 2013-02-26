/*
 * Boost.h
 *
 *  Created on: 25 feb 2013
 *      Author: Cosmic
 */

#ifndef BOOST_H_
#define BOOST_H_

#include "Tower.h"

namespace towers {

class Boost: public towers::Tower {
public:
	Boost();
	virtual ~Boost();

	void update_informationtext();

	TowerType get_type();
	std::string get_type_str();
	bool upgrade(TowerType type); 	   ///< type is the towertype to be upgraded to

private:
	TowerData level_1;
	TowerData level_2;
	TowerData level_3;
};

} /* namespace towers */
#endif /* BOOST_H_ */
