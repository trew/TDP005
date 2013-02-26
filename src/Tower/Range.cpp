/*
 * Range.cpp
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#include "Range.h"

namespace towers {

Range::Range() {
	level = 1;
	level_1 = (TowerData){format_range(2.2f), 35, 3,  5.0f, 40.0f, 15.0f, 0,  40, 0, "./gfx/tower/tower-range-lvl1.png", "./gfx/tower/cannon-range.png"};
	level_2 = (TowerData){format_range(2.8f), 45, 3,  5.5f, 37.0f, 17.0f, 0, 200, 0, "./gfx/tower/tower-range-lvl2.png", "./gfx/tower/cannon-range.png"};
	level_3 = (TowerData){format_range(3.7f),130, 3,  6.0f, 35.0f, 19.0f, 0,   0, 0, "./gfx/tower/tower-range-lvl3.png", "./gfx/tower/cannon-range.png"};
	update_data(level_1);
	sell_value = level_1.cost_buy + 20; // 20 is cost to upgrade from simple tower
	set_base_surf(Sprite::load_image(level_1.base_surface_location));
	set_cannon_surf(Sprite::load_image(level_1.cannon_surface_location));
	update_informationtext();
}

void Range::update_informationtext() {
	clear_informationtext();
	add_row_to_information_text("Type: " + get_type_str());
	add_row_to_information_text("Level: " + get_level_str());
	if (get_damage() > 0)
		add_row_to_information_text("Damage: " + get_damage_str());
	if (get_range() > 0)
		add_row_to_information_text("Range: " + get_range_str());
	if (get_cost_upgrade() > 0)
		add_row_to_information_text("Upgrade cost: " + get_cost_upgrade_str());
}

Range::~Range() {
}

Projectile* Range::spawn_projectile(float x, float y, float angle) {
	return new Projectile("./gfx/tower/ammo/ammo-range.png", x + 15, y + 15,
			angle, projectile_speed, get_damage(), 0, 3000);
}

std::string Range::get_type_str() {
	return "Range Tower";
}

TowerType Range::get_type() {
	return RANGE;
}

bool Range::upgrade(TowerType type) {
	if (type != get_type())
		return false;
	if (level == get_max_level())
		return false;

	level++;
	if (level == 2) {
		sell_value += level_1.cost_upgrade;
		update_data(level_2);
		set_base_surf(Sprite::load_image(level_2.base_surface_location));
		set_cannon_surf(Sprite::load_image(level_2.cannon_surface_location));

	} else if (level == 3) {
		update_data(level_3);
		sell_value += level_2.cost_upgrade;
		set_base_surf(Sprite::load_image(level_3.base_surface_location));
		set_cannon_surf(Sprite::load_image(level_3.cannon_surface_location));

	}
	return false;
}

} /* namespace towers */
