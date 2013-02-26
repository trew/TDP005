/*
 * Basic.cpp
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#include "Basic.h"

namespace towers {

Basic::Basic() {
	level_1 = (TowerData){format_range(1.3f), 20, 3, 350.f, 1000,  600.f, 0,  40, 0, "./gfx/tower/tower-basic-lvl1.png", "./gfx/tower/cannon-basic.png"};
	level_2 = (TowerData){format_range(2.0f), 30, 3, 400.f,  900,  750.f, 0, 200, 0, "./gfx/tower/tower-basic-lvl2.png", "./gfx/tower/cannon-basic.png"};
	level_3 = (TowerData){format_range(2.3f), 70, 3, 500.f,  700, 1000.f, 0,   0, 0, "./gfx/tower/tower-basic-lvl3.png", "./gfx/tower/cannon-basic.png"};
	level = 1;
	update_data(level_1);
	sell_value = level_1.cost_buy + 20; // 20 is cost to upgrade from simple tower
	set_base_surf(Sprite::load_image(level_1.base_surface_location));
	set_cannon_surf(Sprite::load_image(level_1.cannon_surface_location));
	update_informationtext();
}

void Basic::update_informationtext() {
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

Basic::~Basic() {
}

Projectile* Basic::spawn_projectile(Game* g, float x, float y, float angle) {
	return new Projectile(g, "./gfx/tower/ammo/ammo-basic.png", x + 15, y + 15,
			angle, projectile_speed, get_damage(), 0, 3000);
}

std::string Basic::get_type_str() {
	return "Basic Tower";
}

TowerType Basic::get_type() {
	return BASIC;
}

bool Basic::upgrade(TowerType type) {
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
