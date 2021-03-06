/*
 * Tower.cpp
 *
 *  Created on: 17 nov 2010
 *      Author: samuel
 */

#include <Entity/Tower/BaseTower.h>
#include <SDL2_rotozoom.h>
#include <SDL2_gfxPrimitives.h>
#include <cmath>
#include <sstream>
#include <string>
#include <iostream>
#include <Entity/Projectile.h>
#include <Entity/Tower/Tower_Specifics.h>
#include <Core/Game.h>
#include <Entity/Tower/Wall.h>
#include <Entity/Tower/Simple.h>
#include <Entity/Tower/Boost.h>
#include <Entity/Tower/Basic.h>
#include <Entity/Tower/Speed.h>
#include <Entity/Tower/Bomb.h>
#include <Entity/Tower/Range.h>
#include <Core/GameEngine.h>

/*
 * Constructors / Deconstructors
 */
#include <list>
using namespace std;

BaseTower::BaseTower(Game* game, towers::TowerType type, Tile* tile) :
		Sprite(game), tile(tile) {
	if (tile != NULL) {
		x_pos = (float)tile->get_x_pixel_pos();
		y_pos = (float)tile->get_y_pixel_pos();
	}
	height = 40;
	width = 40;

	// Tower specifics
	loaded = false;
	current_target = NULL;
	old_angle = current_angle = 0;
	target_angle = 0;
	rotation_modifier = 0.0;

	// Toggle smoothing for rotozoom
	smoothing = 1;

	renderer = game->getRenderer();

	switch (type)
	{
	case towers::WALL:
	{
		twr_impl = new towers::Wall(renderer);
	}
		break;
	case towers::SIMPLE:
	{
		twr_impl = new towers::Simple(renderer);
		break;
	}
	case towers::BOOST:
	{
		twr_impl = new towers::Boost(renderer);
		break;
	}
	default:
	{
		break;
	}
	}
	reload_timer = twr_impl->get_reloading_time();
}

BaseTower::~BaseTower() {
	delete twr_impl;
	twr_impl = NULL;
}

void BaseTower::drawRange(SDL_Renderer* renderer) {
	drawRange(renderer, x_pos, y_pos);
}

void BaseTower::drawRange(SDL_Renderer* renderer, float x_pos, float y_pos) {
	///Draws a filled circle on the map, showing the range of the tower.

	filledCircleRGBA(renderer, (Sint16)x_pos + TILESIZE / 2, (Sint16)y_pos + TILESIZE / 2, (Sint16)get_range_in_pixels(), 0, 0, 0xFF, 0x11);
	aacircleRGBA(renderer, (Sint16)x_pos + TILESIZE / 2, (Sint16)y_pos + TILESIZE / 2, (Sint16)get_range_in_pixels(), 0x55, 0x55, 0xFF, 0x44);
}

void BaseTower::draw(SDL_Renderer* renderer) {
	draw(renderer, (int)x_pos, (int)y_pos);
}

void BaseTower::draw(SDL_Renderer* renderer, int x, int y) {
	if (!visible)
		return;
	SDL_Rect dest_rect;
	dest_rect.x = x;
	dest_rect.y = y;

	SDL_Texture* baseTexture = twr_impl->getBaseTexture();
	SDL_Texture* baseCannonTexture = twr_impl->getBaseCannonTexture();
	if (baseTexture != NULL) {
		SDL_QueryTexture(baseTexture, NULL, NULL, &dest_rect.w, &dest_rect.h);
		SDL_RenderCopy(renderer, baseTexture, NULL, &dest_rect);
	}

	if (baseCannonTexture != NULL) {
		SDL_QueryTexture(baseCannonTexture, NULL, NULL, &dest_rect.w, &dest_rect.h);
		old_angle = current_angle;
		SDL_Point center;
		center.x = (int)(dest_rect.w / 2.f);
		center.y = (int)(dest_rect.h / 2.f);
		SDL_RenderCopyEx(renderer, baseCannonTexture, NULL, &dest_rect, current_angle, &center, SDL_FLIP_NONE);
	}
}

bool BaseTower::target_in_range(Enemy *target) {
	return get_distance_to_edge(target) <= get_range_in_pixels();
}

void BaseTower::update_angle_to_target() {
	float target_x_pos = current_target->get_center_x();
	float target_y_pos = current_target->get_center_y();

	float distance_to_target = get_distance_to(current_target);
	// modify aim by considering distance to target and target movespeed
	if (current_target->get_direction() == RIGHT)
	{
		target_x_pos += (float)(current_target->get_speed() * distance_to_target / get_projectile_speed());
	}
	else if (current_target->get_direction() == LEFT)
	{
		target_x_pos -= (float)(current_target->get_speed() * distance_to_target / get_projectile_speed());
	}
	else if (current_target->get_direction() == DOWN)
	{
		target_y_pos += (float)(current_target->get_speed() * distance_to_target / get_projectile_speed());
	}
	else if (current_target->get_direction() == UP)
	{
		target_y_pos -= (float)(current_target->get_speed() * distance_to_target / get_projectile_speed());
	}

	float delta_x = get_center_x() - target_x_pos;
	float delta_y = get_center_y() - target_y_pos;
	double radToDeg = 180.f / M_PI;
	target_angle = (atan2(delta_y, delta_x) * radToDeg) - 90;
	
	format_angle(target_angle);
}

void BaseTower::format_angle(double &angle) {
	if (angle > 360.0)
		angle -= 360.0;

	else if (angle < 0.0)
		angle += 360.0;
}

void BaseTower::update_aim(const float &timeStep) {
	update_angle_to_target();

	if (current_angle - target_angle > -(get_rotation_speed() * timeStep)
			&& current_angle - target_angle < get_rotation_speed() * timeStep)
		current_angle = target_angle;

	else if (current_angle != target_angle) {
		if (target_angle > current_angle + 180)
			rotation_modifier = -get_rotation_speed() * timeStep;

		else if (current_angle > target_angle
				&& current_angle - target_angle < 180)
			rotation_modifier = -get_rotation_speed() * timeStep;

		else
			rotation_modifier = get_rotation_speed() * timeStep;
	}
	rotate(rotation_modifier);
}

void BaseTower::rotate(double value) {
	current_angle += value;
}

bool BaseTower::target_in_sight() {
	if (current_angle < target_angle + get_spread() / 2.0f
			&& current_angle > target_angle - get_spread() / 2.0)
		return true;
	else
		return false;
}

void BaseTower::find_new_target() {
	/**
	 * Finds a new target for the tower if it has no current target
	 * or if its current target has gone out of range.
	 */
	EnemyList* enemy_list = get_game()->get_enemies();

	Enemy* closest_object = NULL;
	if (current_target == NULL || get_distance_to(current_target) > get_range_in_pixels()) {
		float closest_distance = 99999.0f;
		int enemy_path_length = 99999;
		for (EnemyList::iterator it = enemy_list->begin(); it != enemy_list->end(); it++) {
			if (target_in_range((*it))) {
				float distanceToEnemy = get_distance_to_edge((*it));

				if (closest_object == NULL)
				{
					closest_object = (*it);
					closest_distance = distanceToEnemy;
					enemy_path_length = (*it)->get_path_length();
				}
				else if ((*it)->get_path_length() < enemy_path_length)
				{
					closest_object = (*it);
					closest_distance = distanceToEnemy;
					enemy_path_length = (*it)->get_path_length();
				}
				else if ((*it)->get_path_length() == enemy_path_length)
				{
					if (distanceToEnemy < closest_distance)
					{
						closest_object = (*it);
						closest_distance = distanceToEnemy;
						enemy_path_length = (*it)->get_path_length();
					}
				}

			}
		}
		current_target = closest_object;
	}
}

void BaseTower::reload(const float &timeStep) {
	if (!loaded) {
		reload_timer -= timeStep;
		if (reload_timer <= 0) {
			reload_timer = get_reloading_time();
			loaded = true;
		}
	}
}

bool BaseTower::is_loaded() {
	return loaded;
}

void BaseTower::try_shoot() {
	/**
	 * Shoots an projectile in the towers current cannon-direction.
	 * The type of projectile being shot depends on the type of the tower.
	 */
	if (current_target != NULL) {
		if (target_in_range(current_target) && target_in_sight()
				&& current_target->get_x() > -(current_target->get_width()) && is_loaded()) {
			ProjectileList* p_list = get_game()->get_projectiles();
			Projectile* p = twr_impl->spawn_projectile(get_game(), x_pos, y_pos, (float)current_angle - 90);
			if (p != NULL) {
				p_list->push_back(p);
				loaded = false;
			}
		}
	}
}

Sprite* BaseTower::get_target() {
	return current_target;
}

void BaseTower::set_target(Enemy* target) {
	current_target = target;
}

void BaseTower::update_boost() {
	///Does a check to see if the current tower is in the range of a boost-tower and should get boosted.

	TowerList* tower_list = get_game()->get_towers();
	TowerList::iterator tower = tower_list->begin();

	if (get_type() == towers::BOOST || get_type() == towers::WALL)
		return;

	if (!tower_list->empty()) {
		std::list<float> modifiers(24);
		for (tower = tower_list->begin(); tower != tower_list->end(); tower++) {
			// If the tower is a boost tower
			if ((*tower)->get_type() == towers::BOOST) {
				// And if the current tower is in the range of the boost tower
				if (get_distance_to((*tower)) <= (*tower)->get_range_in_pixels()) {
					modifiers.push_back((*tower)->get_boostmod());
				}
			}
		}
		modifiers.sort();
		float mod = 1.f;
		int i = 1;
		for (std::list<float>::reverse_iterator it = modifiers.rbegin(); it != modifiers.rend(); it++) {
			mod += (*it) / i;
			i *= 2;
		}
		apply_boost(mod);
	}
}

void BaseTower::update(const float &timeStep) {
	///Updates the state of the current tower which includes rotating, reloading and finding new targets.

	EnemyList* enemy_list = get_game()->get_enemies();
	EnemyList::iterator iter_object = enemy_list->begin();
	reload(timeStep);
	if (!enemy_list->empty()) {
		rotation_modifier = 0.0;
		format_angle(current_angle);
		find_new_target();
		if (current_target != NULL) {
			if (target_in_range(current_target)) {
				update_aim(timeStep);
				try_shoot();
			}
		}
	}
}

void BaseTower::set_selected(bool _selected) {
	selected = _selected;
}

/***** FUNCTIONS TOWARDS THE IMPLEMENTATION INTERFACE *********/
Sprite_List* BaseTower::get_infosprites() {
	return twr_impl != NULL ? twr_impl->get_informationtext() : NULL;
}
int BaseTower::get_cost_upgrade() {
	return twr_impl != NULL ? twr_impl->get_cost_upgrade() : 0;
}
int BaseTower::get_sell_value() {
	return twr_impl != NULL ? twr_impl->get_sell_value() : 0;
}
void BaseTower::set_sell_value(int value) {
	if (twr_impl != NULL)
		twr_impl->set_sell_value(value);
}
int BaseTower::get_base_damage() {
	return twr_impl != NULL ? twr_impl->get_base_damage() : 0;
}
int BaseTower::get_cost_buy() {
	return twr_impl != NULL ? twr_impl->get_cost_buy() : 0;
}
towers::TowerType BaseTower::get_type() {
	return twr_impl != NULL ? twr_impl->get_type() : towers::WALL;
}
int BaseTower::get_level() {
	return twr_impl != NULL ? twr_impl->get_level() : 0;
}
int BaseTower::get_max_level() {
	return twr_impl != NULL ? twr_impl->get_max_level() : 0;
}
float BaseTower::get_range_in_pixels() {
	return twr_impl != NULL ? twr_impl->get_range_in_pixels() : 0;
}
float BaseTower::get_projectile_speed() {
	return twr_impl != NULL ? twr_impl->get_projectile_speed() : 0;
}
float BaseTower::get_rotation_speed() {
	return twr_impl != NULL ? twr_impl->get_rotation_speed() : 0;
}
int BaseTower::get_spread() {
	return twr_impl != NULL ? twr_impl->get_spread() : 0;
}
const float BaseTower::get_reloading_time() {
	return twr_impl != NULL ? twr_impl->get_reloading_time() : 0;
}
float BaseTower::get_boostmod() {
	return twr_impl != NULL ? twr_impl->get_boostmod() : 0;
}
void BaseTower::set_range(float range) {
	if (twr_impl != NULL)
		twr_impl->set_range(range);
}
void BaseTower::set_damage(int dmg) {
	if (twr_impl != NULL)
		twr_impl->set_damage(dmg);
}
void BaseTower::apply_boost(float mod)
{
	if (twr_impl != NULL)
		twr_impl->apply_boost(mod);
}
/***** END: FUNCTIONS TOWARDS THE IMPLEMENTATION INTERFACE *********/

bool BaseTower::upgrade(towers::TowerType type) {
	if (type == get_type())
		return twr_impl->upgrade(type);
	else {
		if (get_type() == towers::SIMPLE) {
			// currently the only tower upgradable
			switch (type)
			{
			case towers::BASIC:
			{
				delete twr_impl;
				twr_impl = new towers::Basic(renderer);
				return true;
			}
			case towers::SPEED:
			{
				delete twr_impl;
				twr_impl = new towers::Speed(renderer);
				return true;
			}
			case towers::RANGE:
			{
				delete twr_impl;
				twr_impl = new towers::Range(renderer);
				return true;
			}
			case towers::BOMB:
			{
				delete twr_impl;
				twr_impl = new towers::Bomb(renderer);
				return true;
			}
			default:
			{
				break;
			}
			return false;
			}
		}
	}
	return false;
}

void BaseTower::set_tile(Tile* _tile) {
	tile = _tile;
}

Tile* BaseTower::get_tile() {
	return tile;
}

double BaseTower::get_angle() {
	return current_angle;
}
