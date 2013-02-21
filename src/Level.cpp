/*
 * Level.cpp
 *
 *  Created on: 1 dec 2010
 *      Author: samuel
 */

#include "Level.h"
#include <iostream>

Level::Level() {
	reset();
}

Level::~Level() {

}

void Level::reset() {
	current_level = 0;
	current_boss_level = 1;
	wave_delay = 30000;		//ms
	wave_start_delay = 0;
}

void Level::compose_new_wave() {
	int amount_of_enemies = 15;
	int enemy_type = 0;
	int enemy_type_range;
	if (current_boss_level < 5) enemy_type_range = current_boss_level;
	else enemy_type_range = 4;
	int pos = 0;
	int prev_pos = 40;
	while (amount_of_enemies > 0) {
		pos = -(prev_pos + (rand() % 25) +15);
		enemy_type = ENEMY + (rand() % enemy_type_range)+1;
		Enemy* new_enemy;
		new_enemy = new Enemy(enemy_type, pos, 7*TILESIZE,30, 30, current_level);
		if(current_level % 5 == 0) {
			enemy_type = ENEMY_BOSS;
			delete new_enemy; new_enemy = NULL;
			new_enemy = new Enemy(enemy_type, pos, 7*TILESIZE,30, 30, current_boss_level);
			amount_of_enemies = 0;
			current_boss_level++;
		}

		enemies_in_wave.push_back(new_enemy);
		amount_of_enemies--;
		prev_pos = -pos;
	}
}

Sprite_List Level::get_new_wave() {
	enemies_in_wave.clear();
	current_level++;
	compose_new_wave();
	last_enemy_sent = false;
	return enemies_in_wave;
}

bool Level::time_to_send_wave() {
	if (!last_enemy_sent) return false;
	if ((wave_start_delay + wave_delay) < SDL_GetTicks()) return true;
	return false;
}

bool Level::last_enemy_is_sent() {
	return last_enemy_sent;
}

void Level::set_last_enemy_sent() {
	wave_start_delay = SDL_GetTicks();
	last_enemy_sent = true;
}

int Level::time_before_next_wave() {
	/**
	 * Returns time before next wave in seconds if below 10s.
	 * Returns -1 if last enemy isn't on screen.
	 */
	if (!last_enemy_sent) return -1;
	if (((wave_start_delay + wave_delay - SDL_GetTicks()) / 1000) < 11)
		return ((wave_start_delay + wave_delay - SDL_GetTicks()) / 1000);
	else
		return -1;
}


int Level::get_level() {
	return current_level;
}
