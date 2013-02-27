/*
 * Bomb.cpp
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#include "Bomb.h"

namespace towers {

Bomb::Bomb() {
	level = 1;
	level_1 = (TowerData){format_range(1.2f), 15, 3, 300.f, 5000, 200.f, 0,  40, 0, "./gfx/tower/tower-bomb-lvl1.png", "./gfx/tower/cannon-bomb.png"};
	level_2 = (TowerData){format_range(1.8f), 20, 3, 350.f, 4500, 250.f, 0, 200, 0, "./gfx/tower/tower-bomb-lvl2.png", "./gfx/tower/cannon-bomb.png"};
	level_3 = (TowerData){format_range(2.2f), 35, 3, 400.f, 4000, 300.f, 0,   0, 0, "./gfx/tower/tower-bomb-lvl3.png", "./gfx/tower/cannon-bomb.png"};
	update_data(level_1);
	sell_value = 25;
	set_base_surf(Sprite::load_image(level_1.base_surface_location));
	set_cannon_surf(Sprite::load_image(level_1.cannon_surface_location));
	update_informationtext();
}

void Bomb::update_informationtext() {
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

Bomb::~Bomb() {
}

Projectile* Bomb::spawn_projectile(Game* g, float x, float y, float angle) {
	return new Projectile(g, "./gfx/tower/ammo/ammo-bomb.png", x + 15, y + 15,
			angle, projectile_speed, get_damage(), 30, 4000);
}

std::string Bomb::get_type_str() {
	return "Bomb Tower";
}

TowerType Bomb::get_type() {
	return BOMB;
}

bool Bomb::upgrade(TowerType type) {
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
		update_informationtext();
	} else if (level == 3) {
		update_data(level_3);
		sell_value += level_2.cost_upgrade;
		set_base_surf(Sprite::load_image(level_3.base_surface_location));
		set_cannon_surf(Sprite::load_image(level_3.cannon_surface_location));
		update_informationtext();
	}
	return true;
}

} /* namespace towers */
