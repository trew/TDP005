/*
 * Speed.cpp
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#include "Speed.h"

namespace towers {

Speed::Speed() {
	level = 1;
	level_1 = (TowerData){1.0f, 12, 3, 400.f, 280, 500.f, 0,  70, 0, "./gfx/tower/tower-speed-lvl1.png", "./gfx/tower/cannon-speed.png"};
	level_2 = (TowerData){1.2f, 15, 3, 500.f, 200, 600.f, 0, 400, 0, "./gfx/tower/tower-speed-lvl2.png", "./gfx/tower/cannon-speed.png"};
	level_3 = (TowerData){1.5f, 30, 3, 600.f, 100, 700.f, 0,   0, 0, "./gfx/tower/tower-speed-lvl3.png", "./gfx/tower/cannon-speed.png"};
	update_data(level_1);
	sell_value = 25;
	set_base_surf(Sprite::load_image(level_1.base_surface_location));
	set_cannon_surf(Sprite::load_image(level_1.cannon_surface_location));
	update_informationtext();
}

void Speed::update_informationtext() {
	clear_informationtext();
	add_row_to_information_text("Type: " + get_type_str());
	add_row_to_information_text("Level: " + itos(get_level()));
	if (get_damage() > 0)
		add_row_to_information_text("Damage: " + itos(get_damage()));
	if (get_range_in_pixels() > 0)
		add_row_to_information_text("Range: " + ftos(get_range()));
	add_row_to_information_text("Reload: " + itos(get_reloading_time()) + "ms");
	if (get_cost_upgrade() > 0)
		add_row_to_information_text("Upgrade cost: " + itos(get_cost_upgrade()));
	add_row_to_information_text("Sell value: " + itos(get_sell_value() / 2));
}

Speed::~Speed() {
}

Projectile* Speed::spawn_projectile(Game* g, float x, float y, float angle) {
	return new Projectile(g, "./gfx/tower/ammo/ammo-speed.png", x + 15, y + 15,
			angle, projectile_speed, get_damage(), 0, 3000);
}

std::string Speed::get_type_str() {
	return "Speed Tower";
}

TowerType Speed::get_type() {
	return SPEED;
}

bool Speed::upgrade(TowerType type) {
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
	update_informationtext();
	return true;
}

} /* namespace towers */
