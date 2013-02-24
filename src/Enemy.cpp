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

int get_grid_position(int pos_x, int pos_y)
{
	/// Convert X,Y-coordinates to Gridspecific integer.
	pos_x = (pos_x - (pos_x % TILESIZE));
	pos_y = (pos_y - (pos_y % TILESIZE));
	return pos_y * (GRIDWIDTH / TILESIZE) + pos_x;
}

Enemy::Enemy(Grid* grid, int new_type, int x_pos_in, int y_pos_in, int width_in, int height_in, int new_level): grid(grid)
{
/*
 * Create new enemy. New enemies have different hitpoints depending on level.
 */
	switch(new_type) {
	case ENEMY_DOG:
		sprite_surf = load_image("./gfx/enemy/enemy-1-30x30.png");
		move_speed = 2;
		max_health = static_cast<int>(dog_health + (dog_health * 0.24) * (new_level-1));
		cost = dog_cost;
		reward_score = dog_cost;
		reward_money = enemy_money_reward;

		break;
	case ENEMY_SNAIL:
		sprite_surf = load_image("./gfx/enemy/enemy-2-30x30.png");
		move_speed = 1;
		max_health = static_cast<int>(snail_health + (snail_health * 0.24) * (new_level-1));
		cost = snail_cost;
		reward_score = snail_cost;
		reward_money = enemy_money_reward;

		break;
	case ENEMY_FISH:
		sprite_surf = load_image("./gfx/enemy/enemy-3-30x30.png");
		move_speed = 1;
		max_health = static_cast<int>(fish_health + (fish_health * 0.24) * (new_level-1));
		cost = fish_cost;
		reward_score = fish_cost;
		reward_money = enemy_money_reward;

		break;
	case ENEMY_PALS:
		sprite_surf = load_image("./gfx/enemy/enemy-4-30x30.png");
		move_speed = 1;
		max_health = static_cast<int>(pals_health + (pals_health * 0.24) * (new_level-1));
		cost = pals_cost;
		reward_score = pals_cost;
		reward_money = enemy_money_reward;

		break;
	case ENEMY_BOSS:
		sprite_surf = load_image("./gfx/enemy/enemy-5-30x30.png");
		move_speed = 1;
		max_health = boss_health + boss_health_mod * (new_level-1);
		cost = 0;
		reward_score = boss_cost;
		reward_money = boss_money_reward;

		break;
	default:
		break;
	}

	health = max_health;

	type = new_type;
	reached_goal = false;
	killed = false;
	x_pos = x_pos_in;
	y_pos = y_pos_in;
	level = new_level;

	x_vel = 0;
	y_vel = 0;

	width = width_in;
	height = height_in;

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
	dest_rect.x = x_pos+5;
	dest_rect.y = y_pos+5;

	SDL_Rect src_rect = {0, 0, height, width};

	SDL_BlitSurface(sprite_surf, &src_rect, dest_surf, &dest_rect);
}

void Enemy::draw_health_bar(SDL_Surface* dest_surf) {
	int margin = 10;
	int x = x_pos + margin;
	int y = y_pos + TILESIZE - 5;
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

bool Enemy::is_on_tile(Tile* tile) {
	return (tile == current_tile || tile == current_destination);
}

bool Enemy::try_update_path(Grid* grid)
{
	/**
	 * Returns true if a path to goal can be found, otherwise returns false.
	 */

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

std::pair<int, int> Enemy::conv_int_to_XY(int in)
{
	///Converts Gridspecific integer to X,Y-coordinate.
	const int x = in % GRIDWIDTH;
	const int y = 40 * (in / GRIDHEIGHT);
	return std::pair<int, int>(x, y);

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

void Enemy::move()
{
	/**
	 * Execute movement in movement direction.
	 */
	Direction dir = move_dir();
	switch (dir)
	{
	case RIGHT:
		x_vel = move_speed;
		y_vel = 0;
		break;

	case LEFT:
		x_vel = -move_speed;
		y_vel = 0;
		break;

	case UP:
		x_vel = 0;
		y_vel = -move_speed;
		break;

	case DOWN:
		x_vel = 0;
		y_vel = move_speed;
		break;
	default:
		break;
	}

	// If enemy movespeed is so fast that it would walk over
	// the set destination-vertex, then we reached the destination, and will
	// start approaching a new tile
	bool switch_tile = false;
	int x_mov_diff = 0;
	int y_mov_diff = 0;

	if (dir == DOWN && y_pos + y_vel > current_destination->get_y_pixel_pos()) {
		y_mov_diff = y_pos + y_vel - current_destination->get_y_pixel_pos();
		switch_tile = true;
		y_pos = current_destination->get_y_pixel_pos();
	}
	else if (dir == UP && y_pos + y_vel < current_destination->get_y_pixel_pos()) {
		y_mov_diff = y_pos + y_vel - current_destination->get_y_pixel_pos();
		switch_tile = true;
		y_pos = current_destination->get_y_pixel_pos();
	}
	else if (dir == UP || dir == DOWN) {
		y_pos += y_vel;
	}
	else if (dir == RIGHT && x_pos + x_vel > current_destination->get_x_pixel_pos()) {
		x_mov_diff = x_pos + x_vel - current_destination->get_x_pixel_pos();
		switch_tile = true;
		x_pos = current_destination->get_x_pixel_pos();
	}
	else if (dir == LEFT && x_pos + x_vel < current_destination->get_x_pixel_pos()) {
		x_mov_diff = x_pos + x_vel - current_destination->get_x_pixel_pos();
		switch_tile = true;
		x_pos = current_destination->get_x_pixel_pos();
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
		x_pos += x_mov_diff;
		y_pos += y_mov_diff;
	} else {
		reached_goal = true;
	}

}

void Enemy::update(EnemyList &enemy_list)
{
	/// Checks if enemy is dead and execute movement.
	if (health < 0)
		killed = true;
	move();
}

void Enemy::take_damage(int dmg)
{
	///Subtract damage from health.
	health -= dmg;
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
