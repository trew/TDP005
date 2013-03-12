/*
 * Enemy.cpp
 *
 *  Created on: 18 nov 2010
 *      Author: samuel
 */

#include "Enemy.h"
#include "Define.h"
#include "Enemy_Specifics.h"
#include <iostream>
#include <SDL/SDL_gfxPrimitives.h>

Enemy::Enemy(Game* game, EnemyType _type, int x_pos_in, int y_pos_in, int width_in, int height_in, int new_level): Sprite(game)
{
/*
 * Create new enemy. New enemies have different hitpoints depending on level.
 */
	switch(_type) {
	case DOG:
		sprite_surf = load_image("./gfx/enemy/enemy-1-30x30.png");
		move_speed = 110;
		max_health = static_cast<int>(dog_health + (dog_health * 0.3) * (new_level-1));
		reward_score = cost = dog_cost;
		reward_money = dog_cost / 2;

		break;
	case SNAIL:
		sprite_surf = load_image("./gfx/enemy/enemy-2-30x30.png");
		move_speed = 45;
		max_health = static_cast<int>(snail_health + (snail_health * 0.3) * (new_level-1));
		reward_score = cost = snail_cost;
		reward_money = snail_cost / 2;

		break;
	case FISH:
		sprite_surf = load_image("./gfx/enemy/enemy-3-30x30.png");
		move_speed = 65;
		max_health = static_cast<int>(fish_health + (fish_health * 0.3) * (new_level-1));
		reward_score = cost = fish_cost;
		reward_money = fish_cost / 2;

		break;
	case PALS:
		sprite_surf = load_image("./gfx/enemy/enemy-4-30x30.png");
		move_speed = 60;
		max_health = static_cast<int>(pals_health + (pals_health * 0.3) * (new_level-1));
		reward_score = cost = pals_cost;
		reward_money = pals_cost / 2;

		break;
	case BOSS:
		sprite_surf = load_image("./gfx/enemy/enemy-5-30x30.png");
		move_speed = 40;
		max_health = boss_health + boss_health_mod * (new_level-1);
		reward_score = cost = boss_cost;
		reward_money = boss_cost / 2;

		break;
	default:
		break;
	}

	health = max_health;

	type = _type;
	reached_goal = false;
	killed = false;
	x_pos = x_pos_in;
	y_pos = y_pos_in;
	level = new_level;

	x_vel = 0;
	y_vel = 0;

	width = width_in;
	height = height_in;

	Grid* grid = game->get_grid();
	current_tile = NULL;
	current_destination = grid->get_start_tile();
	current_path = NULL;
	apply_new_path(grid->get_path(grid->get_start_tile(), grid->get_portal_tile()));
}

Enemy::~Enemy()
{
	/// Nothing here
}

void Enemy::draw(SDL_Surface* dest_surf)
{
	/**
	 * Draw image with compensation for enemy size.
	 */

	if (!visible)
		return;
	SDL_Rect dest_rect;
	dest_rect.x = (Sint16)x_pos;
	dest_rect.y = (Sint16)y_pos;

	SDL_Rect src_rect = {0, 0, height, width};

	SDL_BlitSurface(sprite_surf, &src_rect, dest_surf, &dest_rect);
}

void Enemy::draw_health_bar(SDL_Surface* dest_surf) {
	int margin = 10;
	int x = (int)x_pos + margin / 2;
	int y = (int)y_pos + TILESIZE - 10;
	int width = TILESIZE - 2*margin;
	int height = 2;

	double mod = health / (double)max_health;
	int healthbar_width = (int)(((double)(width - 2)) * mod);

	Uint32 border_color = 0x000000FF; // black
	Uint32 health_color = 0x00FF00FF; // green
	if (mod < 0.2) health_color = 0xFF0000FF; // red
	else if (mod < 0.5) health_color = 0xFF7700FF; // orange
	else if (mod < 0.8) health_color = 0xFFFF00FF; // yellow

	rectangleColor(dest_surf, x, y, x + width, y + height, border_color);
	lineColor(dest_surf, x + 1, y + 1, x + 1 + healthbar_width, y + 1, health_color);
}

EnemyType Enemy::get_type() {
	return type;
}

bool Enemy::is_on_tile(Tile* tile) {
	return (tile == current_tile || tile == current_destination);
}

bool Enemy::try_update_path()
{
	/**
	 * Returns true if a path to goal can be found, otherwise returns false.
	 */
	Grid* grid = get_game()->get_grid();

	//Return true if enemy has reached goal.
	Path* path_list = grid->get_path(current_destination, grid->get_portal_tile());
	if (path_list->size() == 0)
	{
		Tile* dest = grid->get_portal_tile();
		if (current_destination == dest) {
			//Enable update if enemy is at its destination
			path_list->push_back(dest);
			new_path_int = path_list;
			return true;
		}
		return false;
	}
	else
	{
		new_path_int = path_list;
		return true;
	}
}

void Enemy::update_path()
{
	/// Update current pathlist with new path
	apply_new_path(new_path_int);
}

void Enemy::apply_new_path(Path* int_path)
{
	/**
	 * Replace current path with new path.
	 * Converts and int-list to pairs of X,Y-coordinates.
	 */

	if (current_path != NULL) {
		delete current_path;
		current_path = NULL;
	}
	current_path = int_path;
}

Direction Enemy::move_dir()
{
	/**
	 * Determines movement direction depending on where enemy is in relationship to current destination.
	 */
	Direction dir = RIGHT;
	if (current_tile == NULL) dir = RIGHT;
	else if (current_tile->get_position().first < current_destination->get_position().first)
		dir = DOWN;
	else if (current_tile->get_position().second < current_destination->get_position().second)
		dir = RIGHT;
	else if (current_tile->get_position().first > current_destination->get_position().first)
		dir = UP;
	else if (current_tile->get_position().second > current_destination->get_position().second)
		dir = LEFT;
	direction = dir;
	return direction;
}

void Enemy::move(int delta)
{
	/**
	 * Execute movement in movement direction.
	 */
	Direction dir = move_dir();
	switch (dir)
	{
	case RIGHT:
	{
		x_vel = move_speed * get_game()->get_time_modifier();
		y_vel = 0;
		break;
	}
	case LEFT:
	{
		x_vel = -(move_speed * get_game()->get_time_modifier());
		y_vel = 0;
		break;
	}
	case UP:
	{
		x_vel = 0;
		y_vel = -(move_speed * get_game()->get_time_modifier());
		break;
	}
	case DOWN:
	{
		x_vel = 0;
		y_vel = move_speed * get_game()->get_time_modifier();
		break;
	}
	default:
		return;
	}

	// If enemy movespeed is so fast that it would walk over
	// the set destination-vertex, then we reached the destination, and will
	// start approaching a new tile
	bool switch_tile = false;
	float overshoot = 0;

	if (dir == DOWN &&
			get_center_y() + y_vel > current_destination->get_center_y()) {
		switch_tile = true;
		overshoot = get_center_y() + y_vel - current_destination->get_center_y();
		set_center_y(current_destination->get_center_y());
	}
	else if (dir == UP && get_center_y() + y_vel < current_destination->get_center_y()) {
		switch_tile = true;
		overshoot = current_destination->get_center_y() - (get_center_y() + y_vel);
		set_center_y(current_destination->get_center_y());
	}
	else if (dir == UP || dir == DOWN) {
		y_pos += y_vel;
	}
	else if (dir == RIGHT && get_center_x() + x_vel > current_destination->get_center_x()) {
		switch_tile = true;
		overshoot = get_center_x() + x_vel - current_destination->get_center_x();
		set_center_x(current_destination->get_center_x());
	}
	else if (dir == LEFT && get_center_x() + x_vel < current_destination->get_center_x()) {
		switch_tile = true;
		overshoot = current_destination->get_center_x() - (get_center_x() + x_vel);
		set_center_x(current_destination->get_center_x());
	}
	else if (dir == LEFT || dir == RIGHT) {
		x_pos += x_vel;
	}

	if (!switch_tile) return;

	if (!current_path->empty())
	{
		current_tile = current_destination;
		current_destination = current_path->back();
		current_path->pop_back();
		Direction new_dir = move_dir();
		if (new_dir == LEFT)
			x_pos -= overshoot;
		else if (new_dir == RIGHT)
			x_pos += overshoot;
		else if (new_dir == UP)
			y_pos -= overshoot;
		else if (new_dir == DOWN)
			y_pos += overshoot;
	} else {
		reached_goal = true;
	}

}

void Enemy::update(int delta)
{
	/// Checks if enemy is dead and execute movement.
	if (health <= 0)
		killed = true;
	move(delta);
}

void Enemy::take_damage(int dmg)
{
	///Subtract damage from health.
	health -= dmg;
	if (health < 0)
		health = 0;
}

double Enemy::get_speed() {
	return (double)move_speed;
}

Direction Enemy::get_direction() {
	/**
	 * Returns the last direction, does not calculate a new direction. see Enemy::move_dir()
	 */
	return direction;
}

const int Enemy::get_path_length() {
	return current_path->size();
}
