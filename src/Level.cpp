/*
 * Level.cpp
 *
 *  Created on: 1 dec 2010
 *      Author: samuel
 */

#include "Level.h"
#include <iostream>

Level::Level() {
	timer = 0;
	reset();
}

Level::~Level() {

}

void Level::reset() {
	timer = wave_delay;
	current_level = 0;
	current_boss_level = 1;
	last_enemy_sent = true;
	wave_delay = 30000;		//ms
}

void Level::compose_new_wave(Game* game) {
	int amount_of_enemies = 15;
	EnemyType enemy_type = DOG;
	int enemy_type_range;
	if (current_boss_level < 5) enemy_type_range = current_boss_level;
	else enemy_type_range = 3;
	int pos = 0;
	int prev_pos = 40;
	while (amount_of_enemies > 0) {
		pos = -(prev_pos + (rand() % 25) +15);
		int i = (rand() % (enemy_type_range));
		if (i > 3) i = 3;
		enemy_type = (EnemyType)i;
		Enemy* new_enemy;
		new_enemy = new Enemy(game, (EnemyType)enemy_type, pos, 7*TILESIZE + 5,30, 30, current_level);
		if(current_level % 5 == 0) {
			enemy_type = BOSS;
			delete new_enemy; new_enemy = NULL;
			new_enemy = new Enemy(game, (EnemyType)enemy_type, pos, 7*TILESIZE + 5,30, 30, current_boss_level);
			amount_of_enemies = 0;
			current_boss_level++;
		}

		enemies_in_wave.push_back(new_enemy);
		amount_of_enemies--;
		prev_pos = -pos;
	}
}

EnemyList Level::get_new_wave(Game* game) {
	enemies_in_wave.clear();
	current_level++;
	compose_new_wave(game);
	timer = wave_delay;
	last_enemy_sent = false;
	return enemies_in_wave;
}

bool Level::time_to_send_wave(int delta) {
	if (!last_enemy_sent) return false;
	timer -= delta;
	return timer <= 0;
}

bool Level::last_enemy_is_sent() {
	return last_enemy_sent;
}

void Level::set_last_enemy_sent() {
	last_enemy_sent = true;
}

int Level::time_before_next_wave() {
	/**
	 * Returns time before next wave in seconds if below 10s.
	 * Returns -1 if last enemy isn't on screen.
	 */
	if (!last_enemy_sent)
		return -1;
	if (timer < 10000) {
		return timer / 1000 + 1;
	}
	return -1;
}


int Level::get_level() {
	return current_level;
}
