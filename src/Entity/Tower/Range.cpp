/*
 * Range.cpp
 *
 *  Created on: 26 feb 2013
 *      Author: Cosmic
 */

#include <Entity/Tower/Range.h>

namespace towers {

Range::Range(SDL_Renderer* renderer) : Tower(renderer) {
	level = 1;
	level_1 = {2.0f, 30, 3, 250.f, 1.5f, 600.f, 0,  70, 0, "./gfx/tower/tower-range-lvl1.png", "./gfx/tower/cannon-range.png"};
	level_2 = {2.5f, 50, 3, 275.f, 1.2f, 625.f, 0, 400, 0, "./gfx/tower/tower-range-lvl2.png", "./gfx/tower/cannon-range.png"};
	level_3 = {3.0f,100, 3, 300.f, 1.f, 650.f, 0,   0, 0, "./gfx/tower/tower-range-lvl3.png", "./gfx/tower/cannon-range.png"};
	update_data(level_1);
	sell_value = 25;
	setBaseTexture(Sprite::load_image(renderer, level_1.base_surface_location));
	setBaseCannonTexture(Sprite::load_image(renderer, level_1.cannon_surface_location));
	update_informationtext();
}

void Range::update_informationtext() {
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

Range::~Range() {
}

Projectile* Range::spawn_projectile(Game* g, float x, float y, float angle) {
	return new Projectile(g, "./gfx/tower/ammo/ammo-range.png", x + 15, y + 15,
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
