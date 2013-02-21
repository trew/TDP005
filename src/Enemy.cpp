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

const int UP    = 12;
const int DOWN  =  6;
const int RIGHT =  3;
const int LEFT  =  9;
const int DONE  =  0;

int get_grid_position(int pos_x, int pos_y)
{
	/// Convert X,Y-coordinates to Gridspecific integer.
	pos_x = (pos_x - (pos_x % TILESIZE));
	pos_y = (pos_y - (pos_y % TILESIZE));
	return pos_y * (GRIDWIDTH / TILESIZE) + pos_x;
}

Enemy::Enemy(int new_type, int x_pos_in, int y_pos_in, int width_in, int height_in, int new_level)
{
/*
 * Create new enemy. New enemies have different hitpoints depending on level.
 */
	switch(new_type) {
	case ENEMY_DOG:
		sprite_surf = load_image("./gfx/enemy/enemy-1-30x30.png");
		move_speed = 2;
		max_health = dog_health + (dog_health * 0.24) * (new_level-1);
		cost = dog_cost;
		reward_score = dog_cost;
		reward_money = enemy_money_reward;

		break;
	case ENEMY_SNAIL:
		sprite_surf = load_image("./gfx/enemy/enemy-2-30x30.png");
		move_speed = 1;
		max_health = snail_health + (snail_health * 0.24) * (new_level-1);
		cost = snail_cost;
		reward_score = snail_cost;
		reward_money = enemy_money_reward;

		break;
	case ENEMY_FISH:
		sprite_surf = load_image("./gfx/enemy/enemy-3-30x30.png");
		move_speed = 1;
		max_health = fish_health + (fish_health * 0.24) * (new_level-1);
		cost = fish_cost;
		reward_score = fish_cost;
		reward_money = enemy_money_reward;

		break;
	case ENEMY_PALS:
		sprite_surf = load_image("./gfx/enemy/enemy-4-30x30.png");
		move_speed = 1;
		max_health = pals_health + (pals_health * 0.24) * (new_level-1);
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

	current_destination = std::pair<int, int> (0, y_pos);
	new_path(Game::path_control->find_paths(Game::grid_control, get_grid_position(0, y_pos) / TILESIZE, ENEMY_DESTINATION));

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

	SDL_Rect src_rect;
	src_rect.x = 0;
	src_rect.y = height * int(10 - ((health/double(max_health) *10)));
	src_rect.h = height;
	src_rect.w = width;

	SDL_BlitSurface(sprite_surf, &src_rect, dest_surf, &dest_rect);
}

bool Enemy::can_update_path()
{
	/**
	 * Returns true if a path to goal can be found, otherwise returns false.
	 */
	//Return true if enemy has reached goal.
	std::list<int> path_list = Game::path_control->find_paths(Game::grid_control, get_grid_position(current_destination.first,current_destination.second) / TILESIZE, ENEMY_DESTINATION);

	if (path_list.size() == 0)
	{
		if (get_grid_position(current_destination.first, current_destination.second) / TILESIZE == ENEMY_DESTINATION)
			//Enable update if enemy is at its destination
			path_list.push_back(ENEMY_DESTINATION * TILESIZE);
			new_path_int = path_list;
			return true;
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
	new_path(new_path_int);
}

std::pair<int, int> Enemy::conv_int_to_XY(int in)
{
	///Converts Gridspecific integer to X,Y-coordinate.
	const int x = in % GRIDWIDTH;
	const int y = 40 * (in / GRIDHEIGHT);
	return std::pair<int, int>(x, y);

}

void Enemy::new_path(std::list<int> int_path)
{
	/**
	 * Replace current path with new path.
	 * Converts and int-list to pairs of X,Y-coordinates.
	 */

	// Clear any earlier path
	my_path.clear();

	// Convert the incoming list of vectors from their int-format to pair<int X, int Y>
	for (std::list<int>::iterator iter_int_path = int_path.begin(); iter_int_path != int_path.end(); iter_int_path++)
	{
		my_path.push_front(conv_int_to_XY(*iter_int_path));
	}
}

int Enemy::move_dir()
{
	/**
	 * Determines movement direction depending on where enemy is in relationship to current destination.
	 */
	if (y_pos < current_destination.second)
		return DOWN;
	if (x_pos < current_destination.first)
		return RIGHT;
	if (y_pos > current_destination.second)
		return UP;
	if (x_pos > current_destination.first)
		return LEFT;

	return DONE;
}

void Enemy::move()
{
	/**
	 * Execute movement in movement direction.
	 */
	int dir = move_dir();
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

	case DONE:
		if (!my_path.empty())
		{
			current_destination = my_path.front();
			my_path.pop_front();
			break;
		} else {
			reached_goal = true;
		}
		return;
	default:
		break;
	}

	// If enemy movespeed is so fast that it would walk over
	// the set destination-vertex, move to the vertex-position instead
	if (dir == DOWN && y_pos + y_vel > current_destination.second)
		y_pos = current_destination.second;
	else if (dir == UP && y_pos + y_vel < current_destination.second)
		y_pos = current_destination.second;
	else if (dir == UP || dir == DOWN)

		y_pos += y_vel;
	else if (dir == RIGHT && x_pos + x_vel > current_destination.first)
		x_pos = current_destination.first;
	else if (dir == LEFT && x_pos + x_vel < current_destination.first)
		x_pos = current_destination.first;
	else if (dir == LEFT || dir == RIGHT)
		x_pos += x_vel;
}

void Enemy::update(Sprite_List &enemy_list)
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

