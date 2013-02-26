/*
 * Tower.cpp
 *
 *  Created on: 17 nov 2010
 *      Author: samuel
 */

#include "Tower.h"
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <cmath>
#include <sstream>
#include <string>
#include <iostream>
#include "Projectile.h"
#include "Tower_Specifics.h"
#include "Game.h"
#include "Tower/Wall.h"
#include "Tower/Simple.h"
#include "Tower/Boost.h"
#include "Tower/Basic.h"
#include "Tower/Speed.h"
#include "Tower/Bomb.h"
#include "Tower/Range.h"

/*
 * Constructors / Deconstructors
 */
#include <list>
using namespace std;

Tower::Tower(Game* game, towers::TowerType type, Tile* tile) :
		Sprite(game), tile(tile) {
	if (tile != NULL) {
		x_pos = tile->get_x_pixel_pos();
		y_pos = tile->get_y_pixel_pos();
	}
	height = 40;
	width = 40;

	// Tower specifics
	current_target = NULL;
	current_angle = 0;
	target_angle = 0;
	rotation_modifier = 0.0;
	reload_counter = 0;
	boost_modifier = 1;

	// Toggle smoothing for rotozoom
	smoothing = 1;

	switch (type)
	{
	case towers::WALL:
	{
		twr_impl = new towers::Wall();
	}
		break;
	case towers::SIMPLE:
	{
		twr_impl = new towers::Simple();
		break;
	}
	case towers::BOOST:
	{
		twr_impl = new towers::Boost();
		break;
	}
	default:
	{
		break;
	}
	}
}

Tower::~Tower() {
	delete twr_impl;
	twr_impl = NULL;
}

void Tower::draw_range(SDL_Surface* dest_surf) {
	draw_range(dest_surf, x_pos, y_pos);
}

void Tower::draw_range(SDL_Surface* dest_surf, float x_pos, float y_pos) {
	///Draws a filled circle on the map, showing the range of the tower.

	filledCircleColor(dest_surf, (Sint16) x_pos + TILESIZE / 2,
			(Sint16) y_pos + TILESIZE / 2, (Sint16) get_range(), 0x0000FF11);
	circleColor(dest_surf, (Sint16) x_pos + TILESIZE / 2,
			(Sint16) y_pos + TILESIZE / 2, (Sint16) get_range(), 0x5555FF44);
}

void Tower::draw(SDL_Surface* dest_surf) {
	draw(dest_surf, (int)x_pos, (int)y_pos);
}

void Tower::draw(SDL_Surface* dest_surf, int x, int y) {
	if (!visible)
		return;
	SDL_Rect dest_rect;
	dest_rect.x = x;
	dest_rect.y = y;

	SDL_Surface* base_surf = twr_impl->get_base_surface();
	SDL_Surface* cannon_surf = twr_impl->get_cannon_surface();
	if (base_surf != NULL) {
		SDL_BlitSurface(base_surf, NULL, dest_surf, &dest_rect);
	}

	if (cannon_surf != NULL) {
		SDL_Surface *cannon_surf_rotated;
		cannon_surf_rotated = rotozoomSurface(cannon_surf, current_angle, 1, 1);

		SDL_Rect compensation_rect;
		compensation_rect.x = x - (int) ((double) cannon_surf_rotated->w / 2.0)
				+ 20;
		compensation_rect.y = y - (int) ((double) cannon_surf_rotated->h / 2.0)
				+ 20;

		SDL_BlitSurface(cannon_surf_rotated, NULL, dest_surf,
				&compensation_rect);
		SDL_FreeSurface(cannon_surf_rotated);
	}
}

bool Tower::target_in_range(Enemy *target) {
	return get_distance_to(target) <= get_range();
}

void Tower::update_angle_to_target() {
	float target_x_pos = current_target->get_x();
	float target_y_pos = current_target->get_y();

	float distance_to_target = get_distance_to(current_target);
	// modify aim by considering distance to target and target movespeed
	if (current_target->get_direction() == RIGHT)
		target_x_pos += current_target->get_speed() * distance_to_target
				/ get_projectile_speed();
	else if (current_target->get_direction() == LEFT)
		target_x_pos -= current_target->get_speed() * distance_to_target
				/ get_projectile_speed();
	else if (current_target->get_direction() == DOWN)
		target_y_pos += current_target->get_speed() * distance_to_target
				/ get_projectile_speed();
	else if (current_target->get_direction() == UP)
		target_y_pos -= current_target->get_speed() * distance_to_target
				/ get_projectile_speed();

	float delta_x = x_pos - target_x_pos;
	float delta_y = y_pos - target_y_pos;
	double rad = 180.0f / 3.14159f;
	target_angle = atan2(delta_x, delta_y) * rad;

	format_angle(target_angle);
}

void Tower::format_angle(double &angle) {
	if (angle > 360.0)
		angle -= 360.0;

	else if (angle < 0.0)
		angle += 360.0;
}

void Tower::update_aim() {
	update_angle_to_target();

	if (current_angle - target_angle > -(get_rotation_speed())
			&& current_angle - target_angle < get_rotation_speed())
		current_angle = target_angle;

	else if (current_angle != target_angle) {
		if (target_angle > current_angle + 180)
			rotation_modifier = -get_rotation_speed();

		else if (current_angle > target_angle
				&& current_angle - target_angle < 180)
			rotation_modifier = -get_rotation_speed();

		else
			rotation_modifier = get_rotation_speed();
	}
	rotate(rotation_modifier);
}

void Tower::rotate(double value) {
	current_angle += value;
}

bool Tower::target_in_sight() {
	if (current_angle < target_angle + get_spread() / 2.0f
			&& current_angle > target_angle - get_spread() / 2.0)
		return true;
	else
		return false;
}

void Tower::find_new_target(EnemyList &object_list) {
	/**
	 * Finds a new target for the tower if it has no current target
	 * or if its current target has gone out of range.
	 */

	if (current_target == NULL || get_distance_to(current_target) > get_range()) {
		Enemy* closest_object = NULL;
		float closest_distance = 99999.0f;
		for (EnemyList::iterator it = object_list.begin(); it != object_list.end(); it++) {
			if (get_distance_to(*it) < closest_distance) {
				closest_distance = get_distance_to(*it);
				closest_object = (*it);
			}
		}

		if (target_in_range(closest_object))
			current_target = closest_object;
		else
			current_target = NULL;
	}
}

void Tower::reload() {
	if (reload_counter < get_reloading_time()) {
		reload_counter++;
	}

	if (reload_counter >= get_reloading_time())
		reload_counter = 0;
}

bool Tower::is_loaded() {
	if (reload_counter == 0)
		return true;
	else
		return false;
}

void Tower::shoot(ProjectileList &projectile_list) {
	/**
	 * Shoots an projectile in the towers current cannon-direction.
	 * The type of projectile being shot depends on the type of the tower.
	 */
	Projectile* p = twr_impl->spawn_projectile(get_game(), x_pos, y_pos, -(target_angle + 90));
	if (p != NULL)
		projectile_list.push_back(p);
}

void Tower::apply_boost(float mod)
{
	set_range(get_base_range() * mod);
	set_damage((int)((float)get_base_damage() * mod));
}

Sprite* Tower::get_target() {
	return current_target;
}

void Tower::set_target(Enemy* target) {
	current_target = target;
}

void Tower::update_boost(TowerList &tower_list) {
	///Does a check to see if the current tower is in the range of a boost-tower and should get boosted.

	TowerList::iterator iter_object = tower_list.begin();

	float old_boost_percent = boost_modifier;

	if (get_type() == towers::BOOST || get_type() == towers::WALL)
		return;

	if (!tower_list.empty()) {
		// If *this tower is a boost tower, ignore.
		boost_modifier = 1.0f;
		for (iter_object = tower_list.begin();
				iter_object != tower_list.end(); iter_object++) {
			// If the tower is a boost tower
			if ((*iter_object)->get_type() == towers::BOOST) {
				// And if the current tower is in the range of the boost tower
				if (get_distance_to((*iter_object))
						<= (*iter_object)->get_range()) {
					boost_modifier += ((*iter_object)->get_boostmod());
				}
			}
		}
		if (boost_modifier != old_boost_percent) {
			apply_boost(boost_modifier);
		}
	}
}

void Tower::update(EnemyList &enemy_list) {
	///Updates the state of the current tower which includes rotating, reloading and finding new targets.

	EnemyList::iterator iter_object = enemy_list.begin();
	if (!enemy_list.empty()) {
		rotation_modifier = 0.0;
		format_angle(current_angle);
		reload(); // Reloading if not loaded
		find_new_target(enemy_list);
		if (current_target != NULL) {
			if (target_in_range(current_target)) {
				update_aim();
			}
		}
	}
}

void Tower::shoot_if_possible(ProjectileList &projectile_list) {
	/**
	 * Checks if the current target is an enemy, is within the map-grid, is in range,
	 * in sight and if the tower is loaded. If it is, it will shoot.
	 */
	if (current_target != NULL) {
		if (target_in_range(current_target) && target_in_sight()
				&& current_target->get_x() > -(current_target->get_width()) && is_loaded()) {
			shoot(projectile_list);
		}
	}
}

void Tower::set_selected(bool _selected) {
	selected = _selected;
}

/***** FUNCTIONS TOWARDS THE IMPLEMENTATION INTERFACE *********/
Sprite_List* Tower::get_infosprites() {
	return twr_impl != NULL ? twr_impl->get_informationtext() : NULL;
}
int Tower::get_cost_upgrade() {
	return twr_impl != NULL ? twr_impl->get_cost_upgrade() : 0;
}
int Tower::get_sell_value() {
	return twr_impl != NULL ? twr_impl->get_sell_value() : 0;
}
void Tower::set_sell_value(int value) {
	if (twr_impl != NULL)
		twr_impl->set_sell_value((uint)value);
}
uint Tower::get_base_damage() {
	return twr_impl != NULL ? twr_impl->get_base_damage() : 0;
}
int Tower::get_cost_buy() {
	return twr_impl != NULL ? twr_impl->get_cost_buy() : 0;
}
towers::TowerType Tower::get_type() {
	return twr_impl != NULL ? twr_impl->get_type() : towers::WALL;
}
int Tower::get_level() {
	return twr_impl != NULL ? twr_impl->get_level() : 0;
}
int Tower::get_max_level() {
	return twr_impl != NULL ? twr_impl->get_max_level() : 0;
}
float Tower::get_range() {
	return twr_impl != NULL ? twr_impl->get_range() : 0;
}
float Tower::get_base_range() {
	return twr_impl != NULL ? twr_impl->get_base_range() : 0;
}
float Tower::get_projectile_speed() {
	return twr_impl != NULL ? twr_impl->get_projectile_speed() : 0;
}
float Tower::get_rotation_speed() {
	return twr_impl != NULL ? twr_impl->get_rotation_speed() : 0;
}
int Tower::get_spread() {
	return twr_impl != NULL ? twr_impl->get_spread() : 0;
}
float Tower::get_reloading_time() {
	return twr_impl != NULL ? twr_impl->get_reloading_time() : 0;
}
float Tower::get_boostmod() {
	return twr_impl != NULL ? twr_impl->get_boostmod() : 0;
}
void Tower::set_range(float range) {
	if (twr_impl != NULL)
		twr_impl->set_range(range);
}
void Tower::set_damage(int dmg) {
	if (twr_impl != NULL)
		twr_impl->set_damage(dmg);
}
/***** END: FUNCTIONS TOWARDS THE IMPLEMENTATION INTERFACE *********/

bool Tower::upgrade(towers::TowerType type) {
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
				twr_impl = new towers::Basic();
				break;
			}
			case towers::SPEED:
			{
				delete twr_impl;
				twr_impl = new towers::Speed();
				break;
			}
			case towers::RANGE:
			{
				delete twr_impl;
				twr_impl = new towers::Range();
				break;
			}
			case towers::BOMB:
			{
				delete twr_impl;
				twr_impl = new towers::Bomb();
				break;
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

void Tower::set_tile(Tile* _tile) {
	tile = _tile;
}

Tile* Tower::get_tile() {
	return tile;
}

double Tower::get_angle() {
	return current_angle;
}
