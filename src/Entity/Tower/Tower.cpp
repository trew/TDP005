/*
 * Tower.cpp
 *
 *  Created on: 25 feb 2013
 *      Author: Cosmic
 */

#include <Entity/Tower/Tower.h>
#include <Core/Game.h>
#include <UI/Text.h>
#include <sstream>

namespace towers {

Tower::Tower(SDL_Renderer* _renderer) {
	renderer = _renderer;
	spread = 15;
	base_reloading_time = reloading_time = 0;
	projectile_speed = 0.0f;
	boostmod = 0.0f;
	base_damage = damage = 0;
	base_range = range = 0;
	sell_value = 0;
	cost_upgrade = 0;
	rotation_speed = 0;
	level = 0;
	max_level = 0;
	cost_buy = 0;
	baseTexture = NULL;
	cannonTexture = NULL;
	baseCannonTexture = NULL;
	infotext = NULL;
}

Tower::~Tower() {
	SDL_DestroyTexture(baseTexture);
	baseTexture = NULL;
	SDL_DestroyTexture(cannonTexture);
	cannonTexture = NULL;
	SDL_DestroyTexture(baseCannonTexture);
	baseCannonTexture = NULL;
	if (infotext != NULL) {
		for (Sprite_List::iterator it = infotext->begin(); it != infotext->end(); it++) {
			delete (*it);
			(*it) = NULL;
		}
		delete infotext;
		infotext = NULL;
	}
}

Sprite_List* Tower::get_informationtext() {
	return infotext;
}

void Tower::clear_informationtext() {
	if (infotext == NULL) {
		infotext = new Sprite_List;
		return;
	}
	for (Sprite_List::iterator it = infotext->begin(); it != infotext->end(); it++) {
		delete (*it);
		(*it) = NULL;
	}
	infotext->clear();
}

void Tower::add_row_to_information_text(std::string val) {
	int x, y;
	x = 630;
	if (infotext->empty())
		y = 395;
	else
		y = (int)(infotext->back()->get_y() + infotext->back()->get_height() + 5);
	Text* txt = new Text(renderer, val, x, y, Game::standard_font_16);
	infotext->push_back(txt);
}

void Tower::update_informationtext() {
}

SDL_Texture* Tower::getCannonTexture() {
	return cannonTexture;
}

SDL_Texture* Tower::getBaseCannonTexture() {
	return baseCannonTexture;
}

SDL_Texture* Tower::getBaseTexture() {
	return baseTexture;
}

void Tower::setBaseTexture(SDL_Texture* texture) {
	if (texture == baseTexture) return;
	SDL_DestroyTexture(baseTexture);
	baseTexture = texture;
}

void Tower::setBaseCannonTexture(SDL_Texture* texture) {
	if (texture == baseCannonTexture) return;
	SDL_DestroyTexture(baseCannonTexture);
	baseCannonTexture = texture;
	setCannonTexture(NULL);
}

void Tower::setCannonTexture(SDL_Texture* texture) {
	if (texture == cannonTexture) return;
	SDL_DestroyTexture(cannonTexture);
	cannonTexture = texture;
}

Projectile* Tower::spawn_projectile(Game* g, float x, float y, float angle) {
	return NULL;
}

void Tower::update_data(TowerData data) {
	max_level = data.max_level;
	base_range = range = data.range;
	base_damage = damage = data.damage;
	base_reloading_time = reloading_time = data.reloading_time;
	rotation_speed = data.rotation_speed;
	boostmod = data.boostmod;
	cost_buy = data.cost_buy;
	cost_upgrade = data.cost_upgrade;
	projectile_speed = data.projectile_speed;
}

Uint32 Tower::get_level() {
	return level;
}
Uint32 Tower::get_max_level() {
	return max_level;
}
Uint32 Tower::get_damage() {
	return damage;
}
void Tower::set_damage(Uint32 _damage) {
	damage = _damage;
	update_informationtext();
}
float Tower::get_range() {
	return range;
}
float Tower::get_range_in_pixels() {
	return format_range(range);
}
void Tower::set_range(float _range) {
	range = _range;
	update_informationtext();
}
Uint32 Tower::get_base_damage() {
	return base_damage;
}
float Tower::get_base_range() {
	return base_range;
}
float Tower::get_boostmod() {
	return boostmod;
}
Uint32 Tower::get_sell_value() {
	return sell_value;
}
void Tower::set_sell_value(Uint32 value) {
	sell_value = value;
}

Uint32 Tower::get_cost_buy() {
	return cost_buy;
}
Uint32 Tower::get_cost_upgrade() {
	return cost_upgrade;
}
float Tower::get_projectile_speed() {
	return projectile_speed;
}
float Tower::get_rotation_speed() {
	return rotation_speed;
}
int Tower::get_spread() {
	return spread;
}
int Tower::get_reloading_time() {
	return reloading_time;
}
void Tower::set_reloading_time(int time) {
	if (time < 10)
		time = 10;
	reloading_time = time;
	update_informationtext();
}
int Tower::get_base_reloading_time() const {
	return base_reloading_time;
}

void Tower::apply_boost(float mod) {
	set_range(get_base_range() * mod);
	set_damage((int)(get_base_damage() * mod));
	set_reloading_time(get_base_reloading_time() - (int)(get_base_reloading_time() * ((mod - 1.f) / 2)));
}

float Tower::format_range(float range)
{
	/**
	 * Formats the range from a tile based value to a pixel value
	 * i.e. input 1.0f to this function and receive TILESIZE
	 */
	return range != 0.0 ? (range * (float)TILESIZE) + 20.0f : 0;
}

std::string Tower::itos(int i) {
	return ftos((float)i);
}

std::string Tower::ftos(float f) {
	std::stringstream s_stream;
	s_stream << f;
	return s_stream.str();
}
} /* namespace towers */
