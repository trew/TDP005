/*
 * Boost.cpp
 *
 *  Created on: 25 feb 2013
 *      Author: Cosmic
 */

#include <Entity/Tower/Boost.h>

namespace towers {
//                Range

Boost::Boost() {
	level = 1;
	level_1 = {1.2f, 0, 3, 0, 0, 0, 20, 70, 0.1f, "./gfx/tower/tower-boost-lvl1.png", ""};
	level_2 = {1.5f, 0, 3, 0, 0, 0, 20,400, 0.2f, "./gfx/tower/tower-boost-lvl2.png", ""};
	level_3 = {2.0f, 0, 3, 0, 0, 0, 20,  0, 0.3f, "./gfx/tower/tower-boost-lvl3.png", ""};
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
	add_row_to_information_text("Level: " + itos(get_level()));
	if (get_boostmod() > 0)
		add_row_to_information_text("Boost Mod: " + ftos(get_boostmod()));
	if (get_range_in_pixels() > 0)
		add_row_to_information_text("Range: " + ftos(get_range()));
	if (level == 1)
		add_row_to_information_text("Buy cost: " + itos(get_cost_buy()));
	if (get_cost_upgrade() > 0)
		add_row_to_information_text("Upgrade cost: " + itos(get_cost_upgrade()));
	add_row_to_information_text("Sell value: " + itos(get_sell_value() / 2));
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
	update_informationtext();
	return true;
}

} /* namespace towers */
