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

/*
 * Constructors / Deconstructors
 */
#include <list>
using namespace std;

Tower::Tower(int new_type, Tile* tile): tile(tile)
{
	infosprites.clear(); //Make sure it's empty
	base_surf = NULL;
	cannon_surf = NULL;
	if (tile != NULL) {
		x_pos = tile->get_x_pixel_pos();
		y_pos = tile->get_y_pixel_pos();
	}
	type = new_type;
	height = 40;
	width = 40;
	boostmod = 0;

	// Tower specifics
	current_target = NULL;
	current_angle = 0;
	target_angle = 0;
	rotation_modifier = 0.0;
	reload_counter = 0;
	boost_percentage = 100;

	// Toggle smoothing for rotozoom
	smoothing = 1;

	string tmp = "Type: ";

	switch (new_type)
	{
	case TOWER_WALL:
		if ((base_surf = load_image("./gfx/tower/wall.png")) == NULL)
			std::cerr << "FAIL!";

		// Tower Properties
		type = TOWER_WALL;
		level = 0;
		base_range = range = 0;
		rotation_speed = 0.0;
		spread = 0;
		reloading_time = 0;
		damage = 0;
		projectile_speed = 0;
		cost_buy = 1;
		cost_upgrade = 0;

		break;

	case TOWER_BOOST_LEVEL_1:
		base_surf = load_image("./gfx/tower/tower-boost-lvl1.png");

		// Tower Properties
		type = TOWER_BOOST_LEVEL_1;
		level = 1;
		base_range = range = tower_boost_1_range;
		rotation_speed = 0.0;
		spread = 0;
		reloading_time = 0;
		base_damage = damage = 0;
		projectile_speed = 0;
		cost_buy = 20;
		cost_upgrade = tower_boost_1_upgradecost;
		boostmod = tower_boost_1_modifier;

		break;

	case TOWER_BASE:
		cannon_surf = load_image("./gfx/tower/tower-simple.png");

		// Tower Properties
		type = TOWER_BASE;
		level = 1;
		base_range = range = tower_base_range;
		rotation_speed = 5.0;
		spread = 35;
		reloading_time = 20;
		base_damage = damage = tower_base_damage;
		projectile_speed = 6;
		cost_buy = 5;
		cost_upgrade = tower_base_upgradecost;

		break;

	default:
		break;
	}
	sell_value = cost_buy;
	init_info_sprites();
}

Tower::~Tower()
{
	for (std::vector<Sprite*>::iterator it = infosprites.begin(); it != infosprites.end(); it++)
	{
		delete (*it);
	}
	SDL_FreeSurface(cannon_surf);
	SDL_FreeSurface(base_surf);
}

/*
 *
 */

void Tower::init_info_sprites()
{
	int X = 630;
	int Y = 395;
	int row_count = 0;
	int visible_row = 0;

	string tmp = "Type: ";
	if (type == TOWER_WALL)
	{
		infosprites.push_back(new Text(tmp.append("Wall"), X, Y, Game::standard_font_16));
	}
	else if (type == TOWER_BASE)
	{
		infosprites.push_back(new Text(tmp.append("Base Tower"), X, Y, Game::standard_font_16));
	}
	else if (type == TOWER_BOOST_LEVEL_1)
	{
		infosprites.push_back(new Text(tmp.append("Boost Tower"), X, Y, Game::standard_font_16));
	}
	visible_infosprites.push_back(infosprites[row_count]);

	row_count++;
	tmp = "Range: ";
	infosprites.push_back(new Text(tmp.append(get_range_str()), X, 0, Game::standard_font_16));
	if (range > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}

	row_count++;
	tmp = "Level: ";
	infosprites.push_back(new Text(tmp.append(get_level_str()), X, 0, Game::standard_font_16));
	if (level > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}

	row_count++;
	tmp = "Damage: ";
	infosprites.push_back(new Text(tmp.append(get_damage_str()), X, 0, Game::standard_font_16));
	if (damage > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}

	row_count++;
	tmp = "Boost mod: ";
	infosprites.push_back(new Text(tmp.append(conv_int_to_string(boostmod)), X, 0, Game::standard_font_16));
	if (boostmod > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}

	row_count++;
	tmp = "Cost: ";
	infosprites.push_back(new Text(tmp.append(get_cost_buy_str()), X, 0, Game::standard_font_16));
	if (cost_buy > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}

	row_count++;
	tmp = "Upgrade cost: ";
	infosprites.push_back(new Text(tmp.append(get_cost_upgrade_str()), X, 0, Game::standard_font_16));
	if (cost_upgrade > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}
}

void Tower::update_info_sprites()
{
	visible_infosprites.clear();
	int Y = 395;

	int row_count = 0;
	int visible_row = 0;

	string tmp = "Type: ";
	if (type == TOWER_WALL)
	{
		infosprites[row_count]->update_text(tmp.append("Wall"));
	}
	else if (type == TOWER_BASE)
	{
		infosprites[row_count]->update_text(tmp.append("Base Tower"));
	}
	else if (type >= TOWER_BASIC_LEVEL_1 && type <= TOWER_BASIC_LEVEL_3)
	{
		infosprites[row_count]->update_text(tmp.append("Basic Tower"));
	}
	else if (type >= TOWER_SPEED_LEVEL_1 && type <= TOWER_SPEED_LEVEL_3)
	{
		infosprites[row_count]->update_text(tmp.append("Speed Tower"));
	}
	else if (type >= TOWER_BOMB_LEVEL_1 && type <= TOWER_BOMB_LEVEL_3)
	{
		infosprites[row_count]->update_text(tmp.append("Bomb Tower"));
	}
	else if (type >= TOWER_RANGE_LEVEL_1 && type <= TOWER_RANGE_LEVEL_3)
	{
		infosprites[row_count]->update_text(tmp.append("Range Tower"));
	}
	else if (type >= TOWER_BOOST_LEVEL_1 && type <= TOWER_BOOST_LEVEL_3)
	{
		infosprites[row_count]->update_text(tmp.append("Boost Tower"));
	}
	visible_infosprites.push_back(infosprites[row_count]);

	row_count++;
	tmp = "Range: ";
	infosprites[row_count]->update_text(tmp.append(get_range_str()));
	if (range > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}

	row_count++;
	tmp = "Level: ";
	infosprites[row_count]->update_text(tmp.append(get_level_str()));
	if (level > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}

	row_count++;
	tmp = "Damage: ";
	infosprites[row_count]->update_text(tmp.append(get_damage_str()));

	if (damage > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}

	row_count++;
	tmp = "Boost mod: ";
	infosprites[row_count]->update_text(tmp.append(conv_int_to_string(boostmod)));
	if (boostmod > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}

	row_count++;
	tmp = "Cost: ";
	infosprites[row_count]->update_text(tmp.append(get_cost_buy_str()));
	if (cost_buy > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}

	row_count++;
	tmp = "Upgrade cost: ";
	infosprites[row_count]->update_text(tmp.append(get_cost_upgrade_str()));
	if (cost_upgrade > 0)
	{
		visible_row++;
		visible_infosprites.push_back(infosprites[row_count]);
		visible_infosprites.back()->set_y_pos(Y + visible_infosprites.back()->get_height() * visible_row + 5 * visible_row);
	}
}

void Tower::display_range(SDL_Surface* dest_surf, int x_pos, int y_pos)
{
	///Draws a filled circle on the map, showing the range of the tower.

	filledCircleColor(dest_surf, x_pos + TILESIZE / 2, y_pos + TILESIZE / 2, range, 0x0000FF11);
	circleColor(dest_surf, x_pos + TILESIZE / 2, y_pos + TILESIZE / 2, range, 0x5555FF44);
}

void Tower::draw(SDL_Surface* dest_surf)
{
	if (!visible)
		return;
	SDL_Rect dest_rect;
	dest_rect.x = x_pos;
	dest_rect.y = y_pos;

	if (selected && range > 0 && x_pos < GRIDWIDTH)
	{ // If selected, print tower range on the map
		display_range(dest_surf, x_pos, y_pos);
	}

	if (base_surf != NULL)
	{
		SDL_BlitSurface(base_surf, NULL, dest_surf, &dest_rect);
	}

	if (cannon_surf != NULL)
	{
		SDL_Surface *cannon_surf_rotated;
		cannon_surf_rotated = rotozoomSurface(cannon_surf, current_angle, 1, 1);

		SDL_Rect compensation_rect;
		compensation_rect.x = x_pos - (cannon_surf_rotated->w / 2.0) + 20;
		compensation_rect.y = y_pos - (cannon_surf_rotated->h / 2.0) + 20;

		SDL_BlitSurface(cannon_surf_rotated, NULL, dest_surf, &compensation_rect);
		SDL_FreeSurface(cannon_surf_rotated);
	}
}

void Tower::draw(SDL_Surface* dest_surf, int x, int y)
{
	if (!visible)
		return;
	SDL_Rect dest_rect;
	dest_rect.x = x;
	dest_rect.y = y;

	if (base_surf != NULL)
	{
		SDL_BlitSurface(base_surf, NULL, dest_surf, &dest_rect);
	}

	if (cannon_surf != NULL)
	{
		SDL_Surface *cannon_surf_rotated;
		cannon_surf_rotated = rotozoomSurface(cannon_surf, current_angle, 1, 1);

		SDL_Rect compensation_rect;
		compensation_rect.x = x - (cannon_surf_rotated->w / 2.0) + 20;
		compensation_rect.y = y - (cannon_surf_rotated->h / 2.0) + 20;

		SDL_BlitSurface(cannon_surf_rotated, NULL, dest_surf, &compensation_rect);
		SDL_FreeSurface(cannon_surf_rotated);
	}
}

bool Tower::target_in_range(Sprite *target)
{
	return get_distance_to(target) <= range;
}

void Tower::update_angle_to_target()
{
	int delta_x = x_pos - current_target->get_x_pos();
	int delta_y = y_pos - current_target->get_y_pos();
	double rad = 180 / 3.14159;
	target_angle = atan2(delta_x, delta_y) * rad;

	format_angle(target_angle);
}

void Tower::format_angle(double &angle)
{
	if (angle > 360.0)
		angle -= 360.0;

	else if (angle < 0.0)
		angle += 360.0;
}

void Tower::update_aim()
{
	update_angle_to_target();

	if (current_angle - target_angle > -(rotation_speed) && current_angle - target_angle < rotation_speed)
		current_angle = target_angle;

	else if (current_angle != target_angle)
	{
		if (target_angle > current_angle + 180)
			rotation_modifier = -rotation_speed;

		else if (current_angle > target_angle && current_angle - target_angle < 180)
			rotation_modifier = -rotation_speed;

		else
			rotation_modifier = rotation_speed;
	}
	rotate(rotation_modifier);
}

void Tower::rotate(double value)
{
	current_angle += value;
}

bool Tower::target_in_sight()
{
	if (current_angle < target_angle + spread / 2 && current_angle > target_angle - spread / 2)
		return true;
	else
		return false;
}

void Tower::find_new_target(EnemyList &object_list)
{
	/**
	 * Finds a new target for the tower if it has no current target
	 * or if its current target has gone out of range.
	 */

	if (current_target == NULL || get_distance_to(current_target) > range)
	{
		Enemy* closest_object = NULL;
		int closest_distance = 99999;
		EnemyList::iterator iter_object;
		for (iter_object = object_list.begin(); iter_object != object_list.end(); iter_object++)
		{
			if (get_distance_to(*iter_object) < closest_distance)
			{
				closest_distance = get_distance_to(*iter_object);
				closest_object = (*iter_object);
			}
		}

		if (target_in_range(closest_object))
			current_target = closest_object;
		else
			current_target = NULL;
	}
}

void Tower::reload()
{
	if (reload_counter < reloading_time)
	{
		reload_counter++;
	}

	if (reload_counter >= reloading_time)
		reload_counter = 0;
}

bool Tower::is_loaded()
{
	if (reload_counter == 0)
		return true;
	else
		return false;
}

void Tower::shoot(Sprite_List &object_list)
{
	/**
	 * Shoots an projectile in the towers current cannon-direction.
	 * The type of projectile being shot depends on the type of the tower.
	 */

	if (type == TOWER_BASE) {
		object_list.push_back(new Projectile("./gfx/tower/ammo/ammo-basic.png", x_pos + 15, y_pos + 15, -(target_angle + 90), projectile_speed, damage, 0, 3000));
	}
	else if (type >= TOWER_BASIC_LEVEL_1 && type <= TOWER_BASIC_LEVEL_3) {
		object_list.push_back(new Projectile("./gfx/tower/ammo/ammo-basic.png", x_pos + 15, y_pos + 15, -(target_angle + 90), projectile_speed, damage, 0, 3000));
	}
	else if (type >= TOWER_SPEED_LEVEL_1 && type <= TOWER_SPEED_LEVEL_3) {
		object_list.push_back(new Projectile("./gfx/tower/ammo/ammo-speed.png", x_pos + 15, y_pos + 15, -(target_angle + 90), projectile_speed, damage, 0, 3000));
	}
	else if (type >= TOWER_RANGE_LEVEL_1 && type <= TOWER_RANGE_LEVEL_3) {
		object_list.push_back(new Projectile("./gfx/tower/ammo/ammo-range.png", x_pos + 15, y_pos + 15, -(target_angle + 90), projectile_speed, damage, 0, 3000));
	}
	else if (type >= TOWER_BOMB_LEVEL_1 && type <= TOWER_BOMB_LEVEL_3) {
		object_list.push_back(new Projectile("./gfx/tower/ammo/ammo-bomb.png", x_pos + 15, y_pos + 15, -(target_angle + 90), projectile_speed, damage, 30, 4000));
	}
}

void Tower::apply_boost(int old_percentage, int new_percentage)
{
	//range = (range / (old_percentage / 100.0)) * (new_percentage / 100.0);
	//damage = (damage / (old_percentage / 100.0)) * (new_percentage / 100.0);

	range = (base_range * (new_percentage / 100.0));
	damage = (base_damage * (new_percentage / 100.0));
}

bool Tower::has_this_target(Sprite* target)
{
	if (current_target == target)
		return true;
	return false;
}

void Tower::null_current_target()
{
	current_target = NULL;
}

void Tower::update_boost(TowerList &tower_list)
{
	///Does a check to see if the current tower is in the range of a boost-tower and should get boosted.

	TowerList::iterator iter_object = tower_list.begin();

	int old_boost_percent = boost_percentage;

	if (!tower_list.empty() && (*iter_object)->get_type() < ENEMY && (*iter_object)->get_type() > TOWER_WALL)
	{
		// If *this tower is a boost tower, ignore.
		if (!(type >= TOWER_BOOST_LEVEL_1 && type <= TOWER_BOOST_LEVEL_3))
		{
			boost_percentage = 100;
			for (iter_object = tower_list.begin(); iter_object != tower_list.end(); iter_object++)
			{
				// If the tower is a boost tower
				if ((*iter_object)->get_type() == TOWER_BOOST_LEVEL_1 || (*iter_object)->get_type() == TOWER_BOOST_LEVEL_2 || (*iter_object)->get_type() == TOWER_BOOST_LEVEL_3)
				{
					// And if the current tower is in the range of the boost tower
					if (get_distance_to((*iter_object)) <= (*iter_object)->get_range())
					{
						boost_percentage += ((*iter_object)->get_boostmod());
					}
				}
			}
		}
		if (boost_percentage != old_boost_percent)
		{
			apply_boost(old_boost_percent, boost_percentage);
			update_info_sprites();
		}
	}
}

void Tower::update(EnemyList &enemy_list)
{
	///Updates the state of the current tower which includes rotating, reloading and finding new targets.

	EnemyList::iterator iter_object = enemy_list.begin();

	//
	if (!enemy_list.empty() && (*iter_object)->get_type() >= ENEMY && (*iter_object)->get_type() < PROJECTILE)
	{
		rotation_modifier = 0.0;
		format_angle(current_angle);
		reload(); // Reloading if not loaded
		find_new_target(enemy_list);
		if (current_target != NULL)
		{
			if (target_in_range(current_target))
			{
				update_aim();
			}
		}
	}
}

void Tower::shoot_if_possible(Sprite_List &object_list)
{
	/**
	* Checks if the current target is an enemy, is within the map-grid, is in range,
	* in sight and if the tower is loaded. If it is, it will shoot.
	*/
	if (current_target != NULL && current_target->get_type() >= ENEMY && current_target->get_type() < PROJECTILE)
	{
		if (target_in_range(current_target) && target_in_sight() && (current_target->get_x_pos() < GRIDWIDTH && current_target->get_y_pos() < GRIDHEIGHT) && (current_target->get_x_pos() >= 0 && current_target->get_y_pos() >= 0) && is_loaded())
		{
			shoot(object_list);
		}
	}
}

void Tower::set_selected()
{
	selected = true;
}

void Tower::clear_selected()
{
	selected = false;
}

Sprite_List Tower::get_infosprites()
{
	return visible_infosprites;
}

bool Tower::upgrade(int new_type)
{
	if (new_type == TOWER_BASIC_LEVEL_1 && type == TOWER_BASE)
	{ // Basic tower lvl1
		type = TOWER_BASIC_LEVEL_1;
		base_range = range = tower_basic_1_range;
		base_damage = damage = tower_basic_1_damage;
		reloading_time = tower_basic_1_reloadingtime;
		rotation_speed = tower_basic_1_rotationspeed;
		projectile_speed = tower_basic_1_projectilespeed;
		level = 1;
		cost_upgrade = tower_basic_1_upgradecost;

		SDL_FreeSurface(cannon_surf);
		SDL_FreeSurface(base_surf);
		cannon_surf = load_image("./gfx/tower/cannon-basic.png");
		base_surf = load_image("./gfx/tower/tower-basic-lvl1.png");
		apply_boost(100, boost_percentage);
		update_info_sprites();

		return true;
	}

	else if (new_type == TOWER_SPEED_LEVEL_1 && type == TOWER_BASE)
	{ // Speed tower lvl1
		type = TOWER_SPEED_LEVEL_1;
		base_range = range = tower_speed_1_range;
		base_damage = damage = tower_speed_1_damage;
		reloading_time = tower_speed_1_reloadingtime;
		rotation_speed = tower_speed_1_rotationspeed;
		projectile_speed = tower_speed_1_projectilespeed;
		level = 1;
		cost_upgrade = tower_speed_1_upgradecost;

		SDL_FreeSurface(cannon_surf);
		SDL_FreeSurface(base_surf);
		cannon_surf = load_image("./gfx/tower/cannon-speed.png");
		base_surf = load_image("./gfx/tower/tower-speed-lvl1.png");
		apply_boost(100, boost_percentage);
		update_info_sprites();

		return true;
	}

	else if (new_type == TOWER_RANGE_LEVEL_1 && type == TOWER_BASE)
	{ // Range tower lvl1
		type = TOWER_RANGE_LEVEL_1;
		base_range = range = tower_range_1_range;
		base_damage = damage = tower_range_1_damage;
		reloading_time = tower_range_1_reloadingtime;
		rotation_speed = tower_range_1_rotationspeed;
		projectile_speed = tower_range_1_projectilespeed;
		level = 1;
		cost_upgrade = tower_range_1_upgradecost;

		SDL_FreeSurface(cannon_surf);
		SDL_FreeSurface(base_surf);
		cannon_surf = load_image("./gfx/tower/cannon-range.png");
		base_surf = load_image("./gfx/tower/tower-range-lvl1.png");
		apply_boost(100, boost_percentage);
		update_info_sprites();

		return true;
	}

	else if (new_type == TOWER_BOMB_LEVEL_1 && type == TOWER_BASE)
	{ // Bomb tower lvl1
		type = TOWER_BOMB_LEVEL_1;
		base_range = range = tower_bomb_1_range;
		base_damage = damage = tower_bomb_1_damage;
		reloading_time = tower_bomb_1_reloadingtime;
		rotation_speed = tower_bomb_1_rotationspeed;
		projectile_speed = tower_bomb_1_projectilespeed;
		level = 1;
		cost_upgrade = tower_bomb_1_upgradecost;

		SDL_FreeSurface(cannon_surf);
		SDL_FreeSurface(base_surf);
		cannon_surf = load_image("./gfx/tower/cannon-bomb.png");
		base_surf = load_image("./gfx/tower/tower-bomb-lvl1.png");
		apply_boost(100, boost_percentage);
		update_info_sprites();

		return true;
	}

	else if (new_type == 0) // Uppgrade current type
		switch (type)
		{

		//BASIC
		case TOWER_BASIC_LEVEL_1:
			type = TOWER_BASIC_LEVEL_2;
			base_range = range = tower_basic_2_range;
			base_damage = damage = tower_basic_2_damage;
			reloading_time = tower_basic_2_reloadingtime;
			rotation_speed = tower_basic_2_rotationspeed;
			projectile_speed = tower_basic_2_projectilespeed;
			level = 2;
			cost_upgrade = tower_basic_2_upgradecost;

			SDL_FreeSurface(base_surf);
			base_surf = load_image("./gfx/tower/tower-basic-lvl2.png");
			apply_boost(100, boost_percentage);
			update_info_sprites();

			return true;

		case TOWER_BASIC_LEVEL_2:
			type = TOWER_BASIC_LEVEL_3;
			base_range = range = tower_basic_3_range;
			base_damage = damage = tower_basic_3_damage;
			reloading_time = tower_basic_3_reloadingtime;
			rotation_speed = tower_basic_3_rotationspeed;
			projectile_speed = tower_basic_3_projectilespeed;
			level = 3;
			cost_upgrade = 0;

			SDL_FreeSurface(base_surf);
			base_surf = load_image("./gfx/tower/tower-basic-lvl3.png");
			apply_boost(100, boost_percentage);
			update_info_sprites();

			return true;

		case TOWER_BASIC_LEVEL_3:
			return false;

			//SPEED
		case TOWER_SPEED_LEVEL_1:
			type = TOWER_SPEED_LEVEL_2;
			base_range = range = tower_speed_2_range;
			base_damage = damage = tower_speed_2_damage;
			reloading_time = tower_speed_2_reloadingtime;
			rotation_speed = tower_speed_2_rotationspeed;
			projectile_speed = tower_speed_2_projectilespeed;
			level = 2;
			cost_upgrade = tower_speed_2_upgradecost;

			SDL_FreeSurface(base_surf);
			base_surf = load_image("./gfx/tower/tower-speed-lvl2.png");
			apply_boost(100, boost_percentage);
			update_info_sprites();

			return true;

		case TOWER_SPEED_LEVEL_2:
			type = TOWER_SPEED_LEVEL_3;
			base_range = range = tower_speed_3_range;
			base_damage = damage = tower_speed_3_damage;
			reloading_time = tower_speed_3_reloadingtime;
			rotation_speed = tower_speed_3_rotationspeed;
			projectile_speed = tower_speed_3_projectilespeed;
			level = 3;
			cost_upgrade = 0;

			SDL_FreeSurface(base_surf);
			base_surf = load_image("./gfx/tower/tower-speed-lvl3.png");
			apply_boost(100, boost_percentage);
			update_info_sprites();

			return true;

		case TOWER_SPEED_LEVEL_3:
			return false;

			//RANGE
		case TOWER_RANGE_LEVEL_1:
			type = TOWER_RANGE_LEVEL_2;
			base_range = range = tower_range_2_range;
			base_damage = damage = tower_range_2_damage;
			reloading_time = tower_range_2_reloadingtime;
			rotation_speed = tower_range_2_rotationspeed;
			projectile_speed = tower_range_2_projectilespeed;
			level = 2;
			cost_upgrade = tower_range_2_upgradecost;

			SDL_FreeSurface(base_surf);
			base_surf = load_image("./gfx/tower/tower-range-lvl2.png");
			apply_boost(100, boost_percentage);
			update_info_sprites();

			return true;

		case TOWER_RANGE_LEVEL_2:
			type = TOWER_RANGE_LEVEL_3;
			base_range = range = tower_range_3_range;
			base_damage = damage = tower_range_3_damage;
			reloading_time = tower_range_3_reloadingtime;
			rotation_speed = tower_range_3_rotationspeed;
			projectile_speed = tower_range_3_projectilespeed;
			level = 3;
			cost_upgrade = 0;

			SDL_FreeSurface(base_surf);
			base_surf = load_image("./gfx/tower/tower-range-lvl3.png");
			apply_boost(100, boost_percentage);
			update_info_sprites();

			return true;

		case TOWER_RANGE_LEVEL_3:
			return false;

			//BOMB
		case TOWER_BOMB_LEVEL_1:
			type = TOWER_BOMB_LEVEL_2;
			base_range = range = tower_bomb_2_range;
			base_damage = damage = tower_bomb_2_damage;
			reloading_time = tower_bomb_2_reloadingtime;
			rotation_speed = tower_bomb_2_rotationspeed;
			projectile_speed = tower_bomb_2_projectilespeed;
			level = 2;
			cost_upgrade = tower_bomb_2_upgradecost;

			SDL_FreeSurface(base_surf);
			base_surf = load_image("./gfx/tower/tower-bomb-lvl2.png");
			apply_boost(100, boost_percentage);
			update_info_sprites();

			return true;

		case TOWER_BOMB_LEVEL_2:
			type = TOWER_BOMB_LEVEL_3;
			base_range = range = tower_bomb_3_range;
			base_damage = damage = tower_bomb_3_damage;
			reloading_time = tower_bomb_3_reloadingtime;
			rotation_speed = tower_bomb_3_rotationspeed;
			projectile_speed = tower_bomb_3_projectilespeed;
			level = 3;
			cost_upgrade = 0;

			SDL_FreeSurface(base_surf);
			base_surf = load_image("./gfx/tower/tower-bomb-lvl3.png");
			apply_boost(100, boost_percentage);
			update_info_sprites();

			return true;

		case TOWER_BOMB_LEVEL_3:
			return false;

		case TOWER_BOOST_LEVEL_1:
			type = TOWER_BOOST_LEVEL_2;
			base_range = range = tower_boost_2_range;
			boostmod = tower_boost_2_modifier;
			level = 2;
			cost_upgrade = tower_boost_2_upgradecost;

			SDL_FreeSurface(base_surf);
			base_surf = load_image("./gfx/tower/tower-boost-lvl2.png");
			apply_boost(100, boost_percentage);
			update_info_sprites();

			return true;

		case TOWER_BOOST_LEVEL_2:
			type = TOWER_BOOST_LEVEL_3;
			base_range = range = tower_boost_3_range;
			boostmod = tower_boost_3_modifier;
			level = 3;
			cost_upgrade = 0;

			SDL_FreeSurface(base_surf);
			base_surf = load_image("./gfx/tower/tower-boost-lvl3.png");
			apply_boost(100, boost_percentage);
			update_info_sprites();

			return true;

		case TOWER_BOOST_LEVEL_3:
			return false;
		}

	return false;
}

/*
 * Selectors
 */

int Tower::get_damage()
{
	return damage;
}

int Tower::get_boostmod()
{
	return boostmod;
}

int Tower::get_level()
{
	return level;
}

int Tower::get_range()
{
	return range;
}
int Tower::get_cost_buy()
{
	return cost_buy;
}

int Tower::get_cost_upgrade()
{
	return cost_upgrade;
}

std::string Tower::get_type_str()
{
	if (type == TOWER_WALL) {
		return "Wall";
	}
	else if (type == TOWER_BASE) {
		return "Base Tower";
	}
	else if (type >= TOWER_BASIC_LEVEL_1 && type <= TOWER_BASIC_LEVEL_3) {
		return "Basic Tower";
	}
	else if (type >= TOWER_SPEED_LEVEL_1 && type <= TOWER_SPEED_LEVEL_3) {
		return "Speed Tower";
	}
	else if (type >= TOWER_RANGE_LEVEL_1 && type <= TOWER_RANGE_LEVEL_3) {
		return "Range Tower";
	}
	else if (type >= TOWER_BOMB_LEVEL_1 && type <= TOWER_BOMB_LEVEL_3) {
		return "Booster Tower";
	}
	return "";
}

void Tower::set_tile(Tile* _tile) {
	tile = _tile;
}

Tile* Tower::get_tile() {
	return tile;
}

std::string Tower::conv_int_to_string(int i)
{
	std::stringstream s_stream;
	s_stream << i;
	std::string tmp;
	s_stream >> tmp;
	return tmp;
}

std::string Tower::get_range_str()
{
	return conv_int_to_string(range);
}

std::string Tower::get_damage_str()
{
	return conv_int_to_string(damage);
}

std::string Tower::get_level_str()
{
	return conv_int_to_string(level);
}

std::string Tower::get_cost_buy_str()
{
	return conv_int_to_string(cost_buy);
}

std::string Tower::get_cost_upgrade_str()
{
	return conv_int_to_string(cost_upgrade);
}

double Tower::get_angle()
{
	return current_angle;
}

Sprite* Tower::get_current_target()
{
	return current_target;
}

