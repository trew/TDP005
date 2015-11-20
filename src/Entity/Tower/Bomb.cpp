/*
 * Bomb.cpp
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#include <Entity/Tower/Bomb.h>

namespace towers {

Bomb::Bomb() {
	level = 1;
	level_1 = {1.0f, 20, 3, 300.f, 2000, 200.f, 0,  70, 0, "./gfx/tower/tower-bomb-lvl1.png", "./gfx/tower/cannon-bomb.png"};
	level_2 = {1.5f, 35, 3, 350.f, 1800, 250.f, 0, 400, 0, "./gfx/tower/tower-bomb-lvl2.png", "./gfx/tower/cannon-bomb.png"};
	level_3 = {2.0f, 60, 3, 400.f, 1600, 300.f, 0,   0, 0, "./gfx/tower/tower-bomb-lvl3.png", "./gfx/tower/cannon-bomb.png"};
	update_data(level_1);
	sell_value = 25;
	set_base_surf(Sprite::load_image(level_1.base_surface_location));
	set_base_cannon_surf(Sprite::load_image(level_1.cannon_surface_location));
	update_informationtext();
}

void Bomb::update_informationtext() {
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
		set_base_cannon_surf(Sprite::load_image(level_2.cannon_surface_location));
		update_informationtext();
	} else if (level == 3) {
		update_data(level_3);
		sell_value += level_2.cost_upgrade;
		set_base_surf(Sprite::load_image(level_3.base_surface_location));
		set_base_cannon_surf(Sprite::load_image(level_3.cannon_surface_location));
		update_informationtext();
	}
	return true;
}

} /* namespace towers */
