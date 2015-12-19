/*
 * Basic.cpp
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#include <Entity/Tower/Basic.h>

namespace towers {

Basic::Basic(SDL_Renderer* renderer) : Tower(renderer) {
	level_1 = {1.2f, 20, 3, 350.f,  0.7f,  550.f, 0,  70, 0, "./gfx/tower/tower-basic-lvl1.png", "./gfx/tower/cannon-basic.png"};
	level_2 = {1.5f, 35, 3, 400.f,  0.6f,  575.f, 0, 400, 0, "./gfx/tower/tower-basic-lvl2.png", "./gfx/tower/cannon-basic.png"};
	level_3 = {2.0f, 80, 3, 500.f,  0.45f,  600.f, 0,   0, 0, "./gfx/tower/tower-basic-lvl3.png", "./gfx/tower/cannon-basic.png"};
	level = 1;
	update_data(level_1);
	sell_value = 25;
	setBaseTexture(Sprite::load_image(renderer, level_1.base_surface_location));
	setBaseCannonTexture(Sprite::load_image(renderer, level_1.cannon_surface_location));
	update_informationtext();
}

void Basic::update_informationtext() {
	clear_informationtext();
	add_row_to_information_text("Type: " + get_type_str());
	add_row_to_information_text("Level: " + itos(get_level()));
	if (get_damage() > 0)
		add_row_to_information_text("Damage: " + itos(get_damage()));
	if (get_range_in_pixels() > 0)
		add_row_to_information_text("Range: " + ftos(get_range()));
	add_row_to_information_text("Reload: " + itos((int)(get_reloading_time() * 1000.f)) + "ms");
	if (get_cost_upgrade() > 0)
		add_row_to_information_text("Upgrade cost: " + itos(get_cost_upgrade()));
	add_row_to_information_text("Sell value: " + itos(get_sell_value() / 2));
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
		setBaseTexture(Sprite::load_image(renderer, level_2.base_surface_location));
		setBaseCannonTexture(Sprite::load_image(renderer, level_2.cannon_surface_location));
	} else if (level == 3) {
		update_data(level_3);
		sell_value += level_2.cost_upgrade;
		setBaseTexture(Sprite::load_image(renderer, level_3.base_surface_location));
		setBaseCannonTexture(Sprite::load_image(renderer, level_3.cannon_surface_location));
	}
	update_informationtext();
	return true;
}

} /* namespace towers */
