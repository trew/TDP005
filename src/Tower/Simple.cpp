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
	base_range = range = format_range(1.0f);
	rotation_speed = 5.0;
	reloading_time = 20;
	base_damage = damage = 10;
	projectile_speed = 6;
	sell_value = cost_buy = 5;
	cost_upgrade = 20;
	set_cannon_surf(Sprite::load_image("./gfx/tower/tower-simple.png"));
	update_informationtext();
}

void Simple::update_informationtext() {
	clear_informationtext();
	add_row_to_information_text("Type: " + get_type_str());
	add_row_to_information_text("Level: " + get_level_str());
	if (get_damage() > 0)
		add_row_to_information_text("Damage: " + get_damage_str());
	if (get_range() > 0)
		add_row_to_information_text("Range: " + get_range_str());
	if (get_cost_buy() > 0)
		add_row_to_information_text("Buy cost: " + get_cost_buy_str());
	if (get_cost_upgrade() > 0)
		add_row_to_information_text("Upgrade cost: " + get_cost_upgrade_str());
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
