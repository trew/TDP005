/*
 * Simple.cpp
 *
 *  Created on: 25 feb 2013
 *      Author: Cosmic
 */

#include "Simple.h"

namespace towers {

Simple::Simple() {
	level = 1;
	max_level = 1;
	base_range = range = 1.0f;
	rotation_speed = 450.0;
	base_reloading_time = reloading_time = 800;
	base_damage = damage = 15;
	projectile_speed = 300;
	sell_value = cost_buy = 5;
	cost_upgrade = 20;
	set_base_cannon_surf(Sprite::load_image("./gfx/tower/tower-simple.png"));
	update_informationtext();
}

void Simple::update_informationtext() {
	clear_informationtext();
	add_row_to_information_text("Type: " + get_type_str());
	if (get_damage() > 0)
		add_row_to_information_text("Damage: " + itos(get_damage()));
	if (get_range_in_pixels() > 0)
		add_row_to_information_text("Range: " + ftos(get_range()));
	add_row_to_information_text("Reload: " + itos(get_reloading_time()) + "ms");
	if (get_cost_buy() > 0)
		add_row_to_information_text("Buy cost: " + itos(get_cost_buy()));
	if (get_cost_upgrade() > 0)
		add_row_to_information_text("Upgrade cost: " + itos(get_cost_upgrade()));
	add_row_to_information_text("Sell value: " + itos(get_sell_value() / 2));
}

Simple::~Simple() {
}

Projectile* Simple::spawn_projectile(Game* g, float x, float y, float angle) {
	return new Projectile(g, "./gfx/tower/ammo/ammo-basic.png", x + 15, y + 15,
			angle, projectile_speed, get_damage(), 0, 3000);
}

std::string Simple::get_type_str() {
	return "Base Tower";
}

TowerType Simple::get_type() {
	return SIMPLE;
}

bool Simple::upgrade(TowerType type) {
	// cannot upgrade this tower from here.
	return false;
}

} /* namespace towers */
