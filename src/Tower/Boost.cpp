/*
 * Boost.cpp
 *
 *  Created on: 25 feb 2013
 *      Author: Cosmic
 */

#include "Boost.h"

namespace towers {
//                Range

Boost::Boost() {
	level = 1;
	level_1 = (TowerData){format_range(1.2f), 0, 3, 0, 0, 0, 20, 40, 0.2f, "./gfx/tower/tower-boost-lvl1.png", ""};
	level_2 = (TowerData){format_range(1.2f), 0, 3, 0, 0, 0, 20, 40, 0.2f, "./gfx/tower/tower-boost-lvl2.png", ""};
	level_3 = (TowerData){format_range(1.2f), 0, 3, 0, 0, 0, 20, 40, 0.2f, "./gfx/tower/tower-boost-lvl3.png", ""};
	sell_value = level_1.cost_buy;
	update_data(level_1);
	set_base_surf(Sprite::load_image(level_1.base_surface_location));
	update_informationtext();
}

Boost::~Boost() {
}

void Boost::update_informationtext() {
	clear_informationtext();
	add_row_to_information_text("Type: " + get_type_str());
	add_row_to_information_text("Level: " + get_level_str());
	if (get_boostmod() > 0)
		add_row_to_information_text("Boost Mod: " + get_boostmod_str());
	if (get_cost_buy() > 0)
		add_row_to_information_text("Buy cost: " + get_cost_buy_str());
	if (get_cost_upgrade() > 0)
		add_row_to_information_text("Upgrade cost: " + get_cost_upgrade_str());
}

TowerType Boost::get_type() {
	return BOOST;
}

std::string Boost::get_type_str() {
	return "Boost";
}

bool Boost::upgrade(TowerType type) {
	if (type != get_type() || level == get_max_level())
		return false; // can only upgrade to higher level, not to other type

	level++;
	if (level == 2) {
		set_base_surf(Sprite::load_image(level_2.base_surface_location));
		sell_value += level_1.cost_upgrade;
		update_data(level_2);
	} else if (level == 3) {
		set_base_surf(Sprite::load_image(level_3.base_surface_location));
		sell_value += level_2.cost_upgrade;
		update_data(level_3);
	}
	return true;
}

} /* namespace towers */
