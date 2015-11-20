/*
 * Wall.cpp
 *
 *  Created on: 25 feb 2013
 *      Author: Cosmic
 */

#include <Entity/Tower/Wall.h>

namespace towers {

Wall::Wall() {
	sell_value = cost_buy = 1;
	set_base_surf(Sprite::load_image("./gfx/tower/wall.png"));
	update_informationtext();
}

void Wall::update_informationtext() {
	clear_informationtext();
	add_row_to_information_text("Type: " + get_type_str());
	add_row_to_information_text("Cost: " + itos(get_cost_buy()));
	add_row_to_information_text("Sell value: 0");
}

Wall::~Wall() {
}

TowerType Wall::get_type() {
	return WALL;
}

std::string Wall::get_type_str() {
	return "Wall";
}

bool Wall::upgrade(TowerType type) {
	// cannot upgrade this tower
	return false;
}

} /* namespace towers */
